// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchIdleState.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchIdleState : public UWitchBaseState
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void GoNextStateBasedOnDistance();
	
protected:
	
	virtual void OnEnterState(AActor* stateOwner) override;
	void Strafe(float deltaTime);
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;
	UFUNCTION()
	void OnWitchDamaged();

	// ** Idle **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State | Idle", meta = (ClampMin = "0.1", ClampMax = "20.0", UIMin = "0.1", UIMax = "20.0"))
	float minIdleTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State | Idle", meta = (ClampMin = "0.1", ClampMax = "20.0", UIMin = "0.1", UIMax = "20.0"))
	float maxIdleTime = 8.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State | Idle")
	float idleTime = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State | Idle")
	float curIdleTime = 0.f;

	// ** Chase **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State | Chase", meta = (ClampMin = "0.1", ClampMax = "10.0", UIMin = "0.1", UIMax = "10.0"))
	float minChaseTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State | Chase", meta = (ClampMin = "0.1", ClampMax = "10.0", UIMin = "0.1", UIMax = "10.0"))
	float maxChaseTime = 8.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State | Chase")
	float chaseTime = 0.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "State | Chase")
	float curChaseTime = 0.f;
	
	// ** Strafe **
	UPROPERTY(EditAnywhere, Category = "Strafe | Range")
	float closeRangeThreshold = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Strafe | Range")
	float middleRangeThreshold = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Strafe | Duration", meta = (ClampMin = "0.1", ClampMax = "5", UIMin = "0.1", UIMax = "5"))
	float minStrafeChangeDirInterval = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Strafe | Duration", meta = (ClampMin = "0.1", ClampMax = "5", UIMin = "0.1", UIMax = "5"))
	float maxStrafeChangeDirInterval = 2.f;
	UPROPERTY(VisibleAnywhere, Category = "Strafe | Duration")
	float strafeDirChangeInterval = 2.f;
	UPROPERTY(VisibleAnywhere, Category = "Strafe | Duration")
	float curStrafeTime = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Strafe | Movement")
	int32 randomDir = 0;
	UPROPERTY(EditAnywhere, Category = "Strafe | Movement")
	float strafeMovespeedRatio = 0.5f;

	// ** Evade **
	FTimerHandle evadeTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Evade", meta = (ClampMin = "0.1", ClampMax = "4", UIMin = "0.1", UIMax = "4"))
	float minEvadeDelay = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Evade", meta = (ClampMin = "0.1", ClampMax = "5", UIMin = "0.1", UIMax = "5"))
	float maxEvadeDelay = 1.f;
	
	// ** Next state **
	FTimerHandle nextStateHandle;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "5", UIMin = "0.1", UIMax = "5"))
	float beforeNextStatedelay = 0.1f;
	UPROPERTY(VisibleAnywhere)
	bool bNextStateTriggered = false;

private:

	// Is transition to evade state active
	bool bIsEvadeActive = false;
	// Has the witch taken damage
	bool bHasTakenDamage = false;

public:

	// Get close range threshold
	float GetCloseRangeThreshold() const { return closeRangeThreshold; }
	// Get middle range threshold
	float GetMiddleRangeThreshold() const { return middleRangeThreshold; }
};
