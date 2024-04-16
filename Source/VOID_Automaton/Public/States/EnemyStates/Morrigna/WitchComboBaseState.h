// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchComboBaseState.generated.h"

// create struct to store combo montages info
USTRUCT(BlueprintType)
struct FComboMontageInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* animMontage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldDash = false; // should dash before this attacking

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bShouldDash", EditConditionHides = "bShouldDash"))
	float dashVelocity = 0.f; // teleport velocity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bShouldDash", EditConditionHides = "bShouldDash"))
	float offsetBeforePlayer = 0.f; // offset before player location

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bShouldDash", EditConditionHides = "bShouldDash"))
	float secondLater = 0.f; // player location after seconds
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboEnded);

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchComboBaseState : public UWitchBaseState
{
	GENERATED_BODY()

protected:
	
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

	FOnComboEnded OnComboEnded;
	
	UFUNCTION()
	void StartCombo(const TArray<FComboMontageInfo>& comboArray);
	UFUNCTION()
	void PlayCombo(const TArray<FComboMontageInfo>& comboArray);
	UFUNCTION()
	void OnPlayerDamaged(int damageAmount);

protected:

	// ** Combo setting **
	UPROPERTY(EditAnywhere)
	float distanceToTeleport = 200.f;
	UPROPERTY()
	uint8 currentMoveIndex = 0;
	UPROPERTY()
	UAnimMontage* currentMontage;

	// ** Combo delay **
	UPROPERTY(EditAnywhere)
	float comboStartDelay = 0.5f;
	UPROPERTY(EditAnywhere)
	float comboEndDelay = 1.0f;

	// ** Timer **
	UPROPERTY()
	FTimerHandle beforeAttackTimer;
	UPROPERTY()
	FTimerHandle teleportTimer;
	UPROPERTY()
	FTimerHandle comboEndTimer;

	// ** Flag **
	inline static bool bHasHitPlayer = false;
};
