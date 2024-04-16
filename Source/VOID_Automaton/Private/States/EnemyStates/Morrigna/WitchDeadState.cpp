// Fill out your copyright notice in the Description page of Project Settings.

#include "States/EnemyStates/Morrigna/WitchDeadState.h"

#include "Kismet/GameplayStatics.h"

void UWitchDeadState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	// Play death animation
	if(witchRef->deathMontage)
	{
		witchRef->PlayAnimMontage(witchRef->deathMontage);
	}
	
	// Play death VFX
	if(witchRef->deathVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			witchRef,
			witchRef->deathVFX,
			witchRef->GetActorLocation(),
			FRotator::ZeroRotator,
			witchRef->deathVFXScale
			);
	}
}

void UWitchDeadState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
}

void UWitchDeadState::OnExitState()
{
	Super::OnExitState();
}
