// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchComboBaseState.h"

#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"

void UWitchComboBaseState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	// Subscribe to witch damaged player event to decide the next state
	VOIDCharacter->OnDamageTaken.AddDynamic(this, &UWitchComboBaseState::OnPlayerDamaged);
}

void UWitchComboBaseState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
}

void UWitchComboBaseState::OnExitState()
{
	Super::OnExitState();

	// Reset values
	currentMoveIndex = 0;
	currentMontage = nullptr;
	bHasHitPlayer = false;
	
	// Unbind delegate (used in child classes for moving to next state)
	OnComboEnded.Clear();
	VOIDCharacter->OnDamageTaken.RemoveDynamic(this, &UWitchComboBaseState::OnPlayerDamaged);
}

void UWitchComboBaseState::StartCombo(const TArray<FComboMontageInfo>& comboArray)
{
	// Get first & last combo index for combo, can be set in blueprint
	currentMoveIndex = 0; // Should be 0st index of the array
	
	// Delay before starting combo
	GetWorld()->GetTimerManager().SetTimer(beforeAttackTimer, [&]()
	{
		PlayCombo(comboArray);
		
	}, comboStartDelay, false);
}

void UWitchComboBaseState::PlayCombo(const TArray<FComboMontageInfo>& comboArray)
{
	if(witchRef->IsDead() || bPlayerDead) return;
	
	if(witchRef && currentMoveIndex < comboArray.Num() && comboArray[currentMoveIndex].animMontage)
	{
		// Get current combo part
		const auto currentComboPart = comboArray[currentMoveIndex];
	
		// Get the montage from the map using the current index
		currentMontage = currentComboPart.animMontage;
		
		// Get animation length (before moving to next part)
		const float animLength = currentMontage->GetPlayLength() / currentMontage->RateScale;

		// Teleport into melee range before attacking if the combo part requires it
		if(currentComboPart.bShouldDash &&
			UActorUtilities::GetDistanceBetweenActor(witchRef, playerRef) > distanceToTeleport)
		{
			// Set teleport velocity according to the combo part
			witchRef->SetTeleportVelocity(currentComboPart.dashVelocity);

			// Set offset before player
			const float offsetBeforePlayer = comboArray[currentMoveIndex].offsetBeforePlayer;

			// Second later
			const float secondLater = comboArray[currentMoveIndex].secondLater;
			
			// Get player location n second later based on player's current velocity with offset
			const auto targetLocation = UMathUtilities::CalculateFutureActorLocation(
				witchRef, 
				playerRef, 
				secondLater, 
				offsetBeforePlayer
			);

			// Teleport to set location
			witchRef->CombatTeleport(targetLocation);
		}
		
		// Play the attack montage
		witchRef->PlayAnimMontage(currentMontage);

		// Go to next combo part if this is not the last move
		if(currentMoveIndex < comboArray.Num())
		{
			currentMoveIndex += 1;
			
			GetWorld()->GetTimerManager().SetTimer(teleportTimer, [&]()
			{
				// Play the next combo part
				PlayCombo(comboArray);
				
			}, animLength, false);
		}
	}
	else
	{
		OnComboEnded.Broadcast();
	}
}

void UWitchComboBaseState::OnPlayerDamaged(int damageAmount)
{
	bHasHitPlayer = true;
}
