// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "Projectile/ProjectileBase.h"
#include "ProjectileBaseState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UProjectileBaseState : public UStateBase
{
	GENERATED_BODY()

public:

	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void OnExitState() override;
	
protected:

	UPROPERTY(BlueprintReadOnly)
	AProjectileBase* projectileRef = nullptr;
};
