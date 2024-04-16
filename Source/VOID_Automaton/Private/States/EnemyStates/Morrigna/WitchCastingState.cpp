// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchCastingState.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AudioManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Saves/MainGameInstance.h"
#include "VOID_Automaton/DebugMacros.h"

void UWitchCastingState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);
	
	// Random spell number
	const auto minSpellCount = static_cast<uint8>(firstSpell); // need to be first spell
	const auto maxSpellCount = static_cast<uint8>(lastSpell); // need to be last spell
	
	spellIndex = FMath::RandRange(minSpellCount, maxSpellCount);
	spell = static_cast<ESpellList>(spellIndex);
	
	// Set to the spell cast time
	switch (spell)
	{
	case ESpellList::ESL_None:
		break;
	case ESpellList::ESL_NormalProjectile:
		castingTimeMax = witchRef->projectile1CastTime;
		break;	
	case ESpellList::ESL_HomingProjectile:
		castingTimeMax = witchRef->homingCastTime;
		break;
	default:
		break;
	}
	
	// Animation
	if(witchRef->castingMontage)
	{
		witchRef->PlayAnimMontage(witchRef->castingMontage);	
	}

	// VFX
	if(castingVFX)
	{
		castingVFXComponent = UGameplayStatics::SpawnEmitterAtLocation(
		witchRef->GetWorld(),
		castingVFX,
		witchRef->GetActorLocation() + FVector(0.f, 0.f, castingVFXZOffset),
		FRotator::ZeroRotator,
		castingVFXScale
		);
	}

	// SFX
	if(witchRef->castingMurmurSFX)
	{
		if(IManageSingleton* singletonManager = Cast<IManageSingleton>(GetWorld()->GetGameInstance()))
		{
			castingMurmurSound = singletonManager->GetAudioManager_Implementation()->PlaySFX(witchRef->castingMurmurSFX, witchRef);
		}
	}
}

void UWitchCastingState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);

	switch (spell)
	{
	case ESpellList::ESL_None:
		break;
	case ESpellList::ESL_NormalProjectile:
		{
			currentCastingTime += deltaTime;
			if(currentCastingTime >= castingTimeMax)
			{
				// Prevent triggering attack multiple times
				if(bHasTriggered || witchRef->IsDead()) return;

				bHasTriggered = true;

				// Cast n times (can be set at Witch.h)
				witchRef->SpawnNormalProjectile();
				
				castCount++;

				if(castCount < witchRef->projectile1Count)
				{
					currentCastingTime = 0.f;
					bHasTriggered = false;
				}
				else
				{
					// Stop animation
					witchRef->StopAnimMontage(witchRef->castingMontage);
					
					// Go for melee combat after casting spell
					GetWorld()->GetTimerManager().SetTimer(delayAfterCastHandler, [this]()
					{
						if(bPlayerDead) return;
						witchRef->stateManager->SwitchStateByKey("WitchPrepareClose");
				
					}, delayAfterSpellCast, false);	
				}
			}
			break;		
		}
	case ESpellList::ESL_HomingProjectile:
		{
			currentCastingTime += deltaTime;
			if(currentCastingTime >= castingTimeMax)
			{
				// Prevent triggering attack multiple times
				if(bHasTriggered || witchRef->IsDead()) return;

				bHasTriggered = true;

				// Stop casting animation & Start magic attack
				witchRef->StopAnimMontage(witchRef->castingMontage);
				witchRef->SpawnHomingRangeProjectile();

				// Go for melee combat after casting spell
				GetWorld()->GetTimerManager().SetTimer(delayAfterCastHandler, [this]()
				{
					if(bPlayerDead) return;
					witchRef->stateManager->SwitchStateByKey("WitchPrepareClose");
				
				}, delayAfterSpellCast, false);
			}
			
			break;		
		}
	default:
		break;
	}
}

void UWitchCastingState::OnExitState()
{
	Super::OnExitState();

	bHasTriggered = false;
	spell = ESpellList::ESL_None;
	castCount = 0;
	currentCastingTime = 0.f;
	if(castingMurmurSound)
	{
		castingMurmurSound->Stop();
	}
	if(castingVFXComponent)
	{
		// Destroy VFX
		castingVFXComponent->DestroyComponent();
	}
}
