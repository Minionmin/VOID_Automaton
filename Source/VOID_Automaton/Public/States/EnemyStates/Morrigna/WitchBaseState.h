// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Morrigna/Witch.h"
#include "States/EnemyStates/EnemyBaseState.h"
#include "WitchBaseState.generated.h"

class UWitchIdleState;
class AVOIDCharacter;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchBaseState : public UEnemyBaseState
{
	GENERATED_BODY()

protected:

	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

	UFUNCTION()
	void OnPlayerDead();
	UFUNCTION()
	void StartRandomPattern(FString firstStateKey, FString secondStateKey
		, int32 firstStateProbability, FTimerHandle delayHandler, float delay = 0.1f);

protected:

	UPROPERTY(BlueprintReadOnly)
	AWitch* witchRef = nullptr;
	UPROPERTY(BlueprintReadOnly)
	AEnemyAIController* witchAIController = nullptr;
	UPROPERTY(BlueprintReadOnly)
	ACharacter* playerRef = nullptr;
	UPROPERTY(BlueprintReadOnly)
	AVOIDCharacter* VOIDCharacter = nullptr;
	
	// ** Facing Player **
	UPROPERTY(EditAnywhere, Category = "Combat")
	float facePlayerInterpSpeed = 15.f;

	// ステートマシンを切り替えるためのフラグ
	inline static bool bPlayerDead = false;
	
private:
	
	// ** State references **
	// Adding static keyword to make sure that the idleState is shared among all instances of WitchBaseState
	inline static UWitchIdleState* idleState = nullptr;
	
protected:

	// Only allow child classes to get/set the idleState
	static UWitchIdleState* GetIdleState() { return idleState; }
	static void SetIdleState(UWitchIdleState* newState) { idleState = newState; }
};
