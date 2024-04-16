// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchWaitState.h"

#include "MyCustomUnrealLibrary.h"
#include "States/EnemyStates/Morrigna/WitchIdleState.h"

void UWitchWaitState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);
}

void UWitchWaitState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
	
	if(bIsSwitchingState || bPlayerDead) return;
	
	float distanceBetweenPlayer = FVector::Dist(witchRef->GetActorLocation(), playerRef->GetActorLocation());
	
	if(GetIdleState() && distanceBetweenPlayer <= GetIdleState()->GetMiddleRangeThreshold())
	{
		// Switch to idle state when player is in range
		witchRef->stateManager->SwitchStateByKey("WitchIdle");

		bIsSwitchingState = true;
	}
}

void UWitchWaitState::OnExitState()
{
	Super::OnExitState();

	bIsSwitchingState = false;
}
