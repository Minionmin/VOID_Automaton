// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchComboBaseState.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchKickComboState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchKickComboState : public UWitchComboBaseState
{
	GENERATED_BODY()

protected:
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

	UFUNCTION()
	void MoveKickNextState();

	// ** Kick combo **
	UPROPERTY(EditAnywhere, Category = "Kick combo")
	TArray<FComboMontageInfo> kickCombo;
	
	// ** 2nd phase Kick combo **
	UPROPERTY(EditAnywhere, Category = "Kick combo")
	TArray<FComboMontageInfo> secondKickCombo;
};
