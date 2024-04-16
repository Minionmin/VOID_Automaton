// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WitchAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	
	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Initialization", meta = (AllowPrivateAccess = true))
	class AWitch* witchRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Initialization", meta = (AllowPrivateAccess = true))
	class UCharacterMovementComponent* witchMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	FVector velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
    float groundSpeed;

	/*
		States
	*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsFalling = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsDead = false;
	
	/*
		Strafe
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float movementOffsetYaw; // offset yaw used for strafing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float lastMovementOffsetYaw; // offset yaw before the frame we stopped moving

	/*
		Related variables
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float animPlayRate; // animation play rate based on current speed
};
