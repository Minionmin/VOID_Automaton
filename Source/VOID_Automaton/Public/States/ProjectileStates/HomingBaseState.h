// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/HomingProjectile.h"
#include "States/ProjectileStates/ProjectileBaseState.h"
#include "HomingBaseState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UHomingBaseState : public UProjectileBaseState
{
	GENERATED_BODY()

protected:
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AHomingProjectile* homingProjectileRef;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AActor* target = nullptr;
};
