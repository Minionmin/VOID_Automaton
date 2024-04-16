// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Morrigna/WitchAnimInstance.h"

#include "Enemy/Morrigna/Witch.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "VOID_Automaton/DebugMacros.h"

void UWitchAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	witchRef = Cast<AWitch>(TryGetPawnOwner());
	if (witchRef)
	{
		witchMovementComponent = witchRef->GetCharacterMovement();
	}
	
}

void UWitchAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (witchMovementComponent && witchRef)
	{
		// Get character speed
		groundSpeed = UKismetMathLibrary::VSizeXY(witchMovementComponent->Velocity);

		// ** State checking **
		// Get falling state
		bIsFalling = witchMovementComponent->IsFalling();

		// Get dead state
		bIsDead = witchRef->IsDead();

		// Get current velocity
		velocity = witchMovementComponent->Velocity;
		
		// Strafing
		const FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(witchMovementComponent->Velocity); // enemy's movement rotation
		const FRotator aimRotation = witchRef->GetActorRotation(); // actor's facing rotation
		movementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation, aimRotation).Yaw; // movement offset yaw for strafing
		// For stop moving animation (movementOffsetYaw will always be 0 when player stopped moving)
		if (witchMovementComponent->Velocity.Size() > 0.f)
		{
			lastMovementOffsetYaw = movementOffsetYaw;
		}

		// Set animation play rate according to speed (Binding to a blueprint variable)
		animPlayRate = groundSpeed / witchMovementComponent->GetMaxSpeed();
		if (animPlayRate >= 2.f) animPlayRate = 1.5f;
	}
}
