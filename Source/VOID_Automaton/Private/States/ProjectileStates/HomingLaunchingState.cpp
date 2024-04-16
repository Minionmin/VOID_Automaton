// Fill out your copyright notice in the Description page of Project Settings.


#include "States/ProjectileStates/HomingLaunchingState.h"
#include "StateManagerComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VOID_Automaton/DebugMacros.h"

void UHomingLaunchingState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	homingTargetOffset = FMath::FRandRange(-homingTargetOffsetRange, homingTargetOffsetRange);
}

void UHomingLaunchingState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
	
	// ** If there is target then make the projectile move toward it **
	if(target)
	{
		// Calculate location with offset
		const FVector projectileLocation = homingProjectileRef->GetActorLocation();
		const FVector targetLocation = target->GetActorLocation() + target->GetActorRightVector() * homingTargetOffset;
		const FVector dirVector = targetLocation - projectileLocation;
		
		// ** If projectile is close enough to player, we accelerate it and stop updating direction
		if(dirVector.Size() >= minDistanceBeforeAccelerate)
		{
			// For projectiles subtle speed difference
			float homingInterpSpeed;
			if(homingInterpMinSpeed <= homingInterpMaxSpeed) homingInterpSpeed = FMath::FRandRange(homingInterpMinSpeed, homingInterpMaxSpeed);
			else homingInterpSpeed = FMath::FRandRange(homingInterpMaxSpeed, homingInterpMinSpeed);
		
			const FVector interpolatedVector = FMath::VInterpTo(projectileLocation, targetLocation, deltaTime, homingInterpSpeed);
			homingProjectileRef->SetActorLocation(interpolatedVector);	
		}
		else
		{
			// Last update after accelerated
			if(bIsAccelerated) bCanTickState = false;
			
			homingProjectileRef->GetProjectileMovementComponent()->Velocity = dirVector.GetSafeNormal() * speedAfterAccelerated;
			bIsAccelerated = true;
		}
	}
}

void UHomingLaunchingState::OnExitState()
{
	Super::OnExitState();
}
