// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchWaitState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchWaitState : public UWitchBaseState
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

protected:

	// Prevent switching state multiple times
	bool bIsSwitchingState = false;
	
};
