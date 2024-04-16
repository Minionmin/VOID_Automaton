// Fill out your copyright notice in the Description page of Project Settings.


#include "States/ProjectileStates/HomingBaseState.h"

#include "VOID_Automaton/DebugMacros.h"

void UHomingBaseState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	if(!homingProjectileRef)
	{
		homingProjectileRef = Cast<AHomingProjectile>(projectileRef);
	}

	if(!target)
	{
		target = homingProjectileRef->GetProjectileTarget();
	}
}

void UHomingBaseState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
}

void UHomingBaseState::OnExitState()
{
	Super::OnExitState();
}
