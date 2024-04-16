// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchBaseState.h"
#include "WitchComboBaseState.h"
#include "WitchPunchComboState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchPunchComboState : public UWitchComboBaseState
{
	GENERATED_BODY()

protected:
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

	UFUNCTION()
	void MovePunchNextState();

	// ** Kick combo **
	UPROPERTY(EditAnywhere, Category = "Punch combo")
	TArray<FComboMontageInfo> punchCombo;
};
