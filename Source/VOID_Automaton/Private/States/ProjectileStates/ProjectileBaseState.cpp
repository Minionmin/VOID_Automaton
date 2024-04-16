// Fill out your copyright notice in the Description page of Project Settings.


#include "States/ProjectileStates/ProjectileBaseState.h"

#include "VOID_Automaton/DebugMacros.h"

void UProjectileBaseState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	if(!projectileRef)
	{
		projectileRef = Cast<AProjectileBase>(stateOwner);
	}
}

void UProjectileBaseState::OnExitState()
{
	Super::OnExitState();
}
