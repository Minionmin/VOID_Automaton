// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "States/EnemyStates/Morrigna/WitchBaseState.h"
#include "WitchCastingState.generated.h"

UENUM(BlueprintType)
enum class ESpellList : uint8
{
	ESL_None UMETA(DisplayName = "None"),
	ESL_NormalProjectile UMETA(DisplayName = "Normal Projectile"),
	ESL_HomingProjectile UMETA(DisplayName = "Homing Projectile")
};

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWitchCastingState : public UWitchBaseState
{
	GENERATED_BODY()

protected:

	virtual void OnEnterState(AActor* stateOwner) override;
	virtual void TickState(float deltaTime) override;
	virtual void OnExitState() override;
	
private:

	// ** Casting **
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ESpellList firstSpell = ESpellList::ESL_NormalProjectile; // first spell in the enum
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ESpellList lastSpell = ESpellList::ESL_HomingProjectile; // last spell in the enum
	UPROPERTY(VisibleAnywhere)
	ESpellList spell = ESpellList::ESL_None;
	UPROPERTY(VisibleAnywhere)
	uint8 spellIndex;
	UPROPERTY(VisibleAnywhere)
	uint8 castCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	float currentCastingTime = 0.f; // current cast time (move to next state upon casting ended)
	UPROPERTY(VisibleAnywhere)
	float castingTimeMax = 0.f; // get value from the witch class
	
	UPROPERTY(VisibleAnywhere)
	bool bHasTriggered = false;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float delayAfterSpellCast = 0.5f;

	// ** Timer Handles **
	FTimerHandle delayAfterCastHandler;

	// ** VFX **
	UPROPERTY()
	class UParticleSystemComponent* castingVFXComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* castingVFX = nullptr;
	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	FVector castingVFXScale = FVector(1.f);
	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
	float castingVFXZOffset = -50.f;
	
	// ** SFX **
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* castingMurmurSound = nullptr;
};
