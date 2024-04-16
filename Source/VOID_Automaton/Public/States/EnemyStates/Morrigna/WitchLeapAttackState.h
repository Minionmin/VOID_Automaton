// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchLeapAttackState.generated.h"

UENUM(BlueprintType)
enum class EDashSkill : uint8
{
	EDS_None UMETA(Hidden),
	EDS_DashLeap UMETA(DisplayName = "Dash_Leap"),
	EDS_DashLeapSwipe UMETA(DisplayName = "Dash_LeapSwipe"),
	enumCount UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchLeapAttackState : public UWitchBaseState
{
	GENERATED_BODY()

protected:

	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;
	float PlayRandomDashSkill();
	

	// ** Dash skills **
	UPROPERTY(EditAnywhere, Category = "Dash ")
	TMap<EDashSkill, UAnimMontage*> dashSkill;
	
	UPROPERTY(EditAnywhere, Category = "Dash | Leap")
	float dashLeapVelocity = 10000.f;
	
	UPROPERTY()
	FVector midpoint; // midpoint which will be used to calculate curvepoint
	UPROPERTY()
	FVector curvePoint; // curve trajectory
	UPROPERTY()
	FVector targetLocation; // curve destination
	UPROPERTY(EditAnywhere, Category = "Dash | Leap Swipe ")
	float offsetBeforeTarget = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Leap Swipe | Curve")
	float randomCurveRadius = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Leap Swipe | Curve")
	float randomAngle = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Leap Swipe | Curve")
	float curveInterpSpeed = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash | Leap Swipe | Curve")
	float actorCurveInterpSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	float delayUntilNextState = 1.5f;

	// ** Timer handle for next state **
	FTimerHandle dashEndTimer;
};
