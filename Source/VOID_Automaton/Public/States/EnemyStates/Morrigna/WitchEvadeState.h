// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchEvadeState.generated.h"

class UWitchIdleState;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchEvadeState : public UWitchBaseState
{
	GENERATED_BODY()
	
protected:
	void Evade();
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

	UFUNCTION()
	void OnWitchDamaged();
	UFUNCTION()
	void OnEvadeTimerElapsed();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	UAnimMontage* evadeLeftMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	UAnimMontage* evadeRightMontage = nullptr;

	UPROPERTY()
	FVector targetLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	float evadeDistance= 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	float evadeInterpSpeed = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	float nextStateDelay = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade", meta = (ClampMin = "1", ClampMax = "4", UIMin = "1", UIMax = "4"))
	int minEvadeCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade", meta = (ClampMin = "1", ClampMax = "4", UIMin = "1", UIMax = "4"))
	int maxEvadeCount = 3;

private:

	// Timer handle for the evade state transition
	FTimerHandle evadeTimerHandle;
	FTimerHandle nextStateHandle;
	
	// Active only in evade state
	bool bIsEvadeActive = false;
	// Is witch switching state
	bool bIsSwitchingState = false;
	// This time evade count
	int evadeCount = 0;
	// Current evade count
	int currentEvadeCount = 0;
};
