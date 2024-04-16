// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchPrepareCloseState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirstJumpDone);

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchPrepareCloseState : public UWitchBaseState
{
	GENERATED_BODY()

protected:
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

	// ** left, right offset on closing gap to player **
	UPROPERTY(EditAnywhere)
	FVector firstJumpOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector secondJumpOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float firstJumpPercentage = 0.33f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float secondJumpPercentage = 0.66f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "5.0", UIMin = "0.1", UIMax = "5.0"))
	float delayBetweenJump = 1.0f;

	FTimerHandle teleportTimer;

	// ** SFX **
	UPROPERTY()
	class UAudioComponent* laughingSound = nullptr;
	
private:

	// When first jump is done
	FOnFirstJumpDone OnFirstJumpDone;
};
