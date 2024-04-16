// Fill out your copyright notice in the Description page of Project Settings.


#include "VOIDCharacterAnimInstance.h"
#include "VOIDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "VOID_Automaton/DebugMacros.h"

void UVOIDCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	VOIDCharacter = Cast<AVOIDCharacter>(TryGetPawnOwner());
	if (VOIDCharacter)
	{
		VOIDMovementComponent = VOIDCharacter->GetCharacterMovement();
	}
}

void UVOIDCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Because this get updated every frame so we check for nullptr
	if (VOIDMovementComponent && VOIDCharacter)
	{
		// Get character speed
		groundSpeed = UKismetMathLibrary::VSizeXY(VOIDMovementComponent->Velocity);

		// Check character state
		// Is player dead?
		bIsDead = VOIDCharacter->IsDead();
		
		// Is player falling?
		bIsFalling = VOIDMovementComponent->IsFalling();

		// Is player accelerating?
		if (VOIDMovementComponent->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		// Is player aiming?
		bIsAiming = VOIDCharacter->IsAiming();

		FRotator movementRotation = UKismetMathLibrary::MakeRotFromX(VOIDMovementComponent->Velocity); // player's movement rotation
		FRotator aimRotation = VOIDCharacter->GetBaseAimRotation(); // camera rotation (crosshair) for player
		movementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(movementRotation, aimRotation).Yaw; // movement offset yaw for strafing
		// For stop moving animation (movementOffsetYaw will always be 0 when player stopped moving)
		if (VOIDMovementComponent->Velocity.Size() > 0.f)
		{
			lastMovementOffsetYaw = movementOffsetYaw;
		}

		// Set animation play rate according to speed (Binding to a blueprint variable)
		animPlayRate = VOIDMovementComponent->GetMaxSpeed() / VOIDCharacter->GetWalkSpeed();
		if (animPlayRate >= 2.f) animPlayRate = 1.5f;
	}
}

// Making parameters reference-base will tell Blueprint that they are "out" variable
void UVOIDCharacterAnimInstance::GetGroundSurfaceType(bool& OutHasHit, FVector& OutPlayerLocation, TEnumAsByte<EPhysicalSurface>& OutSurface)
{
	//float TraceHeight = 150.f;

	//// Parameters
	//FHitResult HitSurfaceResult;
	//FVector PlayerStartLocation = VOIDCharacter->GetActorLocation();
	//FVector PlayerEndLocation = VOIDCharacter->GetActorLocation() - FVector(0.f, 0.f, TraceHeight);
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(VOIDCharacter);
	//Params.bReturnPhysicalMaterial = true;

	//// Trace for floor type
	//bool bHasHit = GetWorld()->LineTraceSingleByChannel(
	//	HitSurfaceResult,
	//	PlayerStartLocation,
	//	PlayerEndLocation,
	//	ECollisionChannel::ECC_Visibility,
	//	Params
	//);

	//OutHasHit = bHasHit;
	//OutPlayerLocation = PlayerStartLocation;

	//if (bHasHit)
	//{
	//	if (HitSurfaceResult.PhysMaterial != nullptr)
	//	{
	//		OutSurface = HitSurfaceResult.PhysMaterial->SurfaceType;
	//	}
	//}
}