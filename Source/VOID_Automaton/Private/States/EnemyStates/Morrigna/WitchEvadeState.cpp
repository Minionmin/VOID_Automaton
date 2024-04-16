// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchEvadeState.h"

#include "MyCustomUnrealLibrary.h"
#include "States/EnemyStates/Morrigna/WitchIdleState.h"
#include "VOID_Automaton/DebugMacros.h"

void UWitchEvadeState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	// Reset the state switching flag
	bIsSwitchingState = false;
	
	// First time entering the state
	if (bFirstEnter && witchRef)
	{
		// Only bind once
		witchRef->OnTakingDamage.AddDynamic(this, &UWitchEvadeState::OnWitchDamaged);

		bFirstEnter = false;
	}

	// Only call the bound function during evade state only
	bIsEvadeActive = true;

	// Random max evade count this round
	evadeCount = FMath::RandRange(minEvadeCount, maxEvadeCount);

	// Evade into a random direction
	Evade();
	
	// Set a timer for transitioning to the WitchLeapAttackState
	GetWorld()->GetTimerManager().SetTimer(
		evadeTimerHandle,
		this,
		&UWitchEvadeState::OnEvadeTimerElapsed,
		nextStateDelay,
		false);
}

void UWitchEvadeState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);

	// If the witch is evading, interpolate the movement to corresponding direction
	if(witchRef->IsEvading())
	{
		UMathUtilities::InterpolateToLocation(witchRef, targetLocation, evadeInterpSpeed, deltaTime);
	}
}

void UWitchEvadeState::OnExitState()
{
	Super::OnExitState();

	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(evadeTimerHandle);
	// Clear target location
	targetLocation = FVector::ZeroVector;
	// Reset the evade active flag
	bIsEvadeActive = false;
	// Reset evade count
	currentEvadeCount = 0;
}

// If witch still taking damage, evade then reset the timer
void UWitchEvadeState::OnWitchDamaged()
{
	// If the evade state is not active, do not evade
	if (!bIsEvadeActive)
	{
		return;
	}
	
	const auto witchAnimInstance = witchRef->GetEnemyAnimInstance();
	// If variables are nullptr, don't do anything
	// If the witch is in evading animation, don't do anything
	if (!witchAnimInstance || !evadeLeftMontage || witchAnimInstance->Montage_IsPlaying(evadeLeftMontage))
	{
		return;
	}

	// Evade into a random direction
	Evade();
	
	// Reset the timer
	GetWorld()->GetTimerManager().SetTimer(
		evadeTimerHandle,
		this,
		&UWitchEvadeState::OnEvadeTimerElapsed,
		nextStateDelay,
		false);
	
}

// Evade into a random direction
void UWitchEvadeState::Evade()
{
	// If we're already in the process of switching states, do nothing
	if (bIsSwitchingState || witchRef->IsDead())
	{
		return;
	}
	
	// Increment the evade count
	currentEvadeCount++;

	// If the evade count has reached the threshold, switch states and return
	if (currentEvadeCount > evadeCount)
	{
		// Set the flag to true to prevent multiple state switches
		bIsSwitchingState = true;

		// If player is out of closeRangeThreshold, witch will go for dash attack
		// Otherwise, witch will go for close combat
		const float distanceBetweenPlayer = UActorUtilities::GetDistanceBetweenActor(witchRef, playerRef);

		if(GetIdleState() &&
			distanceBetweenPlayer < GetIdleState()->GetCloseRangeThreshold())
		{
			StartRandomPattern("WitchPunchCombo", "WitchKickCombo", 50
				, nextStateHandle, 0.1f);	
		}
		else
		{
			//witchRef->stateManager->SwitchStateByKey("WitchLeapAttack");
			StartRandomPattern("WitchLeapAttack", "WitchCasting", 50
			, nextStateHandle, 0.1f);
		}
		
		return;
	}
	
	// Randomly choose a dodge direction
	const bool bEvadeLeft = FMath::RandBool();

	// Play the appropriate evade montage
	// Evading flag will be set in the animation notify
	if(witchRef)
	{
		if (bEvadeLeft && evadeLeftMontage)
		{
			witchRef->PlayAnimMontage(evadeLeftMontage);
			
			// Get actor left vector and multiply by evade distance
			const FVector leftVector = witchRef->GetActorRightVector() * -evadeDistance;
			// Set the target location to the left of the witch
			targetLocation = witchRef->GetActorLocation() + leftVector;
		}
		else if (!bEvadeLeft && evadeRightMontage)
		{
			witchRef->PlayAnimMontage(evadeRightMontage);
			// Get actor right vector and multiply by evade distance
			const FVector rightVector = witchRef->GetActorRightVector() * evadeDistance;
			// Set the target location to the right of the witch
			targetLocation = witchRef->GetActorLocation() + rightVector;
		}
	}
}

void UWitchEvadeState::OnEvadeTimerElapsed()
{
	// If we're already in the process of switching states, do nothing
	// In the case evade animation takes longer than the timer
	if (bIsSwitchingState)
	{
		return;
	}

	// Set the flag to true to prevent multiple state switches
	bIsSwitchingState = true;
	
	// The witch is not taking damage, transition to the WitchLeapAttackState
	witchRef->stateManager->SwitchStateByKey("WitchLeapAttack");
}
