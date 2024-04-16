// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingBaseState.h"
#include "HomingLaunchingState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UHomingLaunchingState : public UHomingBaseState
{
	GENERATED_BODY()

protected:
	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;

protected:

	/*
		State Configures
	*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsAccelerated = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Configures", meta = (ClampMin = "0.1", ClampMax = "4.0", UIMin = "0.1", UIMax = "4.0"))
	float homingInterpMinSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Configures", meta = (ClampMin = "0.1", ClampMax = "4.0", UIMin = "0.1", UIMax = "4.0"))
	float homingInterpMaxSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Configures", meta = (ClampMin = "200", ClampMax = "2000", UIMin = "200", UIMax = "2000"))
	float minDistanceBeforeAccelerate = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Configures", meta = (ClampMin = "500", ClampMax = "10000", UIMin = "500", UIMax = "10000"))
	float speedAfterAccelerated = 700.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Configures", meta = (ClampMin = "0", ClampMax = "500", UIMin = "0", UIMax = "500"))
	float homingTargetOffsetRange = 200.f;
	UPROPERTY()
	float homingTargetOffset = 0.f;
	
};
