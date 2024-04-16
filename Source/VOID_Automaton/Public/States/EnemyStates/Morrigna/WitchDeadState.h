// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchDeadState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchDeadState : public UWitchBaseState
{
	GENERATED_BODY()

protected:

	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;
};
