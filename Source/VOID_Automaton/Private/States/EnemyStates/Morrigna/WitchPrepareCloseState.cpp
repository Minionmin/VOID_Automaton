// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchPrepareCloseState.h"

#include "MyCustomUnrealLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AudioManager.h"
#include "Saves/MainGameInstance.h"
#include "VOID_Automaton/DebugMacros.h"

void UWitchPrepareCloseState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);
	
	// Play laughing SFX while attacking
	if(witchRef->laughingSFX)
	{
		if(IManageSingleton* singletonManager = Cast<IManageSingleton>(GetWorld()->GetGameInstance()))
		{
			laughingSound = singletonManager->GetAudioManager_Implementation()->PlaySFX(witchRef->laughingSFX, witchRef);
		}
	}
	
	const float overallDistanceToPlayer = UActorUtilities::GetDistanceBetweenActor(witchRef, playerRef);
	const float firstJumpDistance = overallDistanceToPlayer - (overallDistanceToPlayer * firstJumpPercentage); // 1/3 of distance
	const float secondJumpDistance = overallDistanceToPlayer - (overallDistanceToPlayer * secondJumpPercentage); // 2/3 of distance
	
	// First jump 1/3 of the distance toward player
	const FVector firstDestination = UActorUtilities::GetLocationBeforeActor(witchRef, playerRef
		, firstJumpDistance) + firstJumpOffset;
	witchRef->NormalTeleport(firstDestination);

	// Second jump 2/3 of the distance toward player
	GetWorld()->GetTimerManager().SetTimer(teleportTimer, [&,secondJumpDistance]()
	{
		
		const FVector secondDestination = UActorUtilities::GetLocationBeforeActor(witchRef, playerRef
			, secondJumpDistance) + secondJumpOffset;
		witchRef->NormalTeleport(secondDestination);
		
		// Random which attack state should switch next (PunchCombo 50% or KickCombo 50%)
		StartRandomPattern("WitchPunchCombo", "WitchKickCombo"
			, 50, teleportTimer, delayBetweenJump);
		
	}, delayBetweenJump, false);

}

void UWitchPrepareCloseState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
}

void UWitchPrepareCloseState::OnExitState()
{
	Super::OnExitState();
}
