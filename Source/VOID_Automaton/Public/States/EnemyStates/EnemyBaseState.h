// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "Enemy/Enemy.h"
#include "EnemyBaseState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UEnemyBaseState : public UStateBase
{
	GENERATED_BODY()

public:

	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void OnExitState() override;
	
public:

	UPROPERTY(BlueprintReadOnly)
	AEnemy* enemyRef = nullptr;
};
