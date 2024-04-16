// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/ProjectileBase.h"
#include "NiagaraProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API ANiagaraProjectileBase : public AProjectileBase
{
	GENERATED_BODY()

public:	

	ANiagaraProjectileBase();
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
	UNiagaraSystem* impactVFX;
	
protected:

	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UNiagaraComponent* projectileNiagara;
};
