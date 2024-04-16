// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchIdleState.h"

#include "MyCustomUnrealLibrary.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "VOID_Automaton/DebugMacros.h"

void UWitchIdleState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner); // Get witchRef
	
	// Only bind once and set the reference to this WitchIdleState
	if (bFirstEnter && witchRef)
	{
		witchRef->OnTakingDamage.AddDynamic(this, &UWitchIdleState::OnWitchDamaged);
		
		// Set the idleState reference in the WitchBaseState class
		SetIdleState(this);

		// Reset the player dead flag
		bPlayerDead = false;
			
		// Then set to wait state
		witchRef->stateManager->SwitchStateByKey("WitchWait");
		
		bFirstEnter = false;
	}

	// Reset evasion flag
	bIsEvadeActive = true;
	// Reset damage flag
	bHasTakenDamage = false;
	
	// Disable witch's moving trail effect when idle
	witchRef->DisableTrailVFX();
	
	// Get idleTime for this round
	idleTime = FMath::FRandRange(minIdleTime, maxIdleTime);
	curIdleTime = idleTime;

	// Get chaseTime for this round
	chaseTime = FMath::FRandRange(minChaseTime, maxChaseTime);
	
	// Get first strafe interval
	strafeDirChangeInterval = FMath::FRandRange(minStrafeChangeDirInterval, maxStrafeChangeDirInterval);
	// Get right/left vector and move in that direction
	do
	{
		randomDir = FMath::RandRange(-1, 1);
	}
	while (randomDir == 0);
}

void UWitchIdleState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
	
	idleTime -= deltaTime;

	// Strafe if in range, get closer if not
	Strafe(deltaTime);
	
	// Go to next state upon idle time ended
	if(idleTime <= 0.f)
	{
		GoNextStateBasedOnDistance();
	}
}

void UWitchIdleState::OnExitState()
{
	Super::OnExitState();
	
	// Stop any movement currently happening when exiting state
	witchRef->GetAIController()->StopMovement();
	// Reset state transition flag
	bNextStateTriggered = false;
	// Reset evasion flag
	bIsEvadeActive = false;
	// Reset damage flag
	bHasTakenDamage = false;
	// Reset all strafing related variables
	randomDir = 0;
	curStrafeTime = 0.f;
	curChaseTime = 0.f;

	// Enable witch's moving trail effect when exit idle
	witchRef->EnableTrailVFX();
}

void UWitchIdleState::Strafe(float deltaTime)
{
	const float distanceBetweenPlayer = UActorUtilities::GetDistanceBetweenActor(witchRef, playerRef);
	// If outside of range, move closer to player
	if(distanceBetweenPlayer >= closeRangeThreshold)
	{
		const FVector toPlayer = (playerRef->GetActorLocation() - witchRef->GetActorLocation()).GetSafeNormal();
		witchRef->GetMovementComponent()->AddInputVector(toPlayer);

		// If we've been chasing for too long, switch state instantly
		if(curChaseTime >= chaseTime)
		{
			GoNextStateBasedOnDistance();
		}
		else
		{
			curChaseTime += deltaTime;
		}
	}
	// If in range, strafe around player
	else
	{
		// Check if it's time to change direction
		if(curStrafeTime >= strafeDirChangeInterval)
		{
			// Change dir
			randomDir = -randomDir;
			
			curStrafeTime = 0.f;
			strafeDirChangeInterval = FMath::FRandRange(minStrafeChangeDirInterval, maxStrafeChangeDirInterval);
		}
		else
		{
			// because enemy will always face player, it will automatically move in circular motion
			witchRef->GetMovementComponent()->AddInputVector(witchRef->GetActorRightVector() * randomDir * strafeMovespeedRatio);
			curStrafeTime += deltaTime;
		}
	}
}

void UWitchIdleState::GoNextStateBasedOnDistance()
{
	if(bNextStateTriggered || witchRef->IsDead()) return;

	// Prevent getting called multiple times in Tick
	bNextStateTriggered = true;
	
	const float distanceBetweenPlayer = UActorUtilities::GetDistanceBetweenActor(witchRef, playerRef);

	if(distanceBetweenPlayer < closeRangeThreshold) // If in aggro range
	{
		// Go for close combat
		witchRef->stateManager->SwitchStateByKey("WitchPrepareClose");
	}
	else if(distanceBetweenPlayer >= closeRangeThreshold) // If in middle range
	{
		// Go for dash attack / cast spell
		StartRandomPattern("WitchLeapAttack", "WitchCasting", 50
			, nextStateHandle, beforeNextStatedelay);
	}
}

// This function will be called when the witch takes damage
void UWitchIdleState::OnWitchDamaged()
{
	// If the witch is not in idle state, do not transition to evade state
	// Or if the witch has already taken damage, do not reset the timer
	if (!bIsEvadeActive || bHasTakenDamage)
	{
		return;
	}
	
	// Mark that the witch has taken damage
	bHasTakenDamage = true;
	
	// Transition to the EvadeState
	if(witchRef)
	{
		// Generate a random delay value
		const float delay = FMath::RandRange(minEvadeDelay, maxEvadeDelay);

		// Set a timer for entering the evade state
		GetWorld()->GetTimerManager().SetTimer(evadeTimerHandle, [&]()
		{
			if(witchRef->IsDead()) return;
			
			witchRef->stateManager->SwitchStateByKey("WitchEvade");
		}, delay, false);
	}
}
