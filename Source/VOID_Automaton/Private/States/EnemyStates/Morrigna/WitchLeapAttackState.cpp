// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchLeapAttackState.h"

#include "MyCustomUnrealLibrary.h"


void UWitchLeapAttackState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);
	
	// Anim length for next state delay
	float animLength = PlayRandomDashSkill();
	
	// Go to next state automatically after the montage finished
	StartRandomPattern("WitchPunchCombo", "WitchKickCombo", 50
		, dashEndTimer, animLength);
}

void UWitchLeapAttackState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);

	// For Dash LeapingSwipe (boolean is set in animation notify)
	if(witchRef->IsLeaping())
	{
		// Start interpolating location when in leaping state
		UMathUtilities::CurveToLocation(witchRef, curvePoint, targetLocation
			, deltaTime, curveInterpSpeed, actorCurveInterpSpeed);
	}
}

void UWitchLeapAttackState::OnExitState()
{
	Super::OnExitState();

	// Set teleport velocity back
	witchRef->SetTeleportVelocity(witchRef->GetOriginalTeleportVelocity());
}

float UWitchLeapAttackState::PlayRandomDashSkill()
{
	// Return played animation length
	float animLength = 0.f;
	
	// Get random dash skill
	switch (const auto skillEnum = UEnumUtilities::GetRandomEnumValue<EDashSkill>())
	{
	case EDashSkill::EDS_DashLeap:
		{
			// Set flying velocity to "dash leap" parameter
			witchRef->SetTeleportVelocity(dashLeapVelocity);
		
			// Find this animation montage from map using the enum
			const auto dashLeapMontage = dashSkill[skillEnum];
		
			// Play the animation montage
			witchRef->PlayAnimMontage(dashLeapMontage);
		
			// Get animation length (before moving to next state)
			animLength = dashLeapMontage->GetPlayLength() / dashLeapMontage->RateScale;
		
			break;
		}
	case EDashSkill::EDS_DashLeapSwipe:
		{
			// Get dash target location with offset
			targetLocation = UMathUtilities::CalculateFutureActorLocation(
				witchRef
				, playerRef
				, 1.f
				, offsetBeforeTarget);
	
			midpoint = UMathUtilities::CalculateMidpoint(witchRef->GetActorLocation(), targetLocation, 0.6f);
			curvePoint = UMathUtilities::GetCurveControlPoint(midpoint, targetLocation, randomAngle, randomCurveRadius);
			
			// Find this animation montage from map using the enum
			const auto dashLeapSwipeMontage = dashSkill[skillEnum];

			// Play the animation montage
			witchRef->PlayAnimMontage(dashLeapSwipeMontage);
	
			// Get animation length (before moving to next state)
			animLength = dashLeapSwipeMontage->GetPlayLength() / dashLeapSwipeMontage->RateScale;
			
			break;	
		}
	default:
		break;
	}

	return animLength;
}