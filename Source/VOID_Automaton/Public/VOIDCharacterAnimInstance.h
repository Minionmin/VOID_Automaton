// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VOIDCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UVOIDCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	/*
		AnimNotify
	*/
	// ** Socket ** //
	UFUNCTION(BlueprintCallable)
	void GetGroundSurfaceType(bool& OutHasHit, FVector& OutPlayerLocation, TEnumAsByte<EPhysicalSurface>& OutSurface);

	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Initialization", meta = (AllowPrivateAccess = true))
	class AVOIDCharacter* VOIDCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Initialization", meta = (AllowPrivateAccess = true))
	class UCharacterMovementComponent* VOIDMovementComponent;

	/*
		Character states
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsFalling = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsAccelerating = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsAiming = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bIsDead = false;

	/*
		Related variable
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float groundSpeed; // player's ground speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float animPlayRate; // animation play rate based on player's speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float movementOffsetYaw; // offset yaw used for strafing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General", meta = (AllowPrivateAccess = true))
	float lastMovementOffsetYaw; // offset yaw before the frame we stopped moving

public:

	// Get ground speed
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetGroundSpeed() const { return groundSpeed; }
};
