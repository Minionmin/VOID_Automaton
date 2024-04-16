// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProjectileBase.h"
#include "GameFramework/Actor.h"
#include "CascadeProjectileBase.generated.h"

#define PROJECTILE_COLLISION_CHANNEL ECC_GameTraceChannel1

class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class VOID_AUTOMATON_API ACascadeProjectileBase : public AProjectileBase
{
	GENERATED_BODY()
	
public:	

	ACascadeProjectileBase();
	virtual void Tick(float DeltaTime) override;

	virtual FGameplayTagContainer GetGameplayTagContainer_Implementation() override;
	
protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;

public:

	/*
		VFX
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* impactVFX;
	
protected:

	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* projectileCascade;
};
