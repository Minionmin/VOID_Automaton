// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingBaseState.h"
#include "HomingPreparingState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UHomingPreparingState : public UHomingBaseState
{
	GENERATED_BODY()

protected:
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;
};
