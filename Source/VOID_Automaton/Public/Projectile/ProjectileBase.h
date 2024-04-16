// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HasGameplayTag.h"
#include "ProjectileBase.generated.h"

#define PROJECTILE_COLLISION_CHANNEL ECC_GameTraceChannel1

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class VOID_AUTOMATON_API AProjectileBase : public AActor, public IHasGameplayTag
{
	GENERATED_BODY()
	
public:	

	AProjectileBase();
	virtual void Tick(float DeltaTime) override;

	virtual FGameplayTagContainer GetGameplayTagContainer_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void TurnOnCollision();
	UFUNCTION(BlueprintCallable)
	void TurnOffCollision();
	UFUNCTION(BlueprintCallable)
	void SetProjectileVelocity(FVector targetLocation, float velocity, bool bShouldOffset = false, float offset = 0.f);
	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
protected:

	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* sphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* projectileMovementComponent;

	/*
		Tag
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer actorTagsContainer;

	/*
		Stat 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float baseDamage = 10.f;

	/*
		Flag
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bShouldOffsetVelocity = false;
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bShouldShakeCamera = true;
	
	/*
		General 
	*/
	UPROPERTY(VisibleInstanceOnly)
	AActor* projectileOwner = nullptr; // Projectile owner
	UPROPERTY(EditAnywhere, meta = (EditCondition = bShouldShakeCamera))
	TSubclassOf<UCameraShakeBase> projectileCamShakeClass;

public:
	
	UFUNCTION(BlueprintCallable)
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return projectileMovementComponent; }
	UFUNCTION(BlueprintCallable)
	AActor* GetProjectileOwner() const { return projectileOwner; }
	UFUNCTION(BlueprintCallable)
	void SetProjectileOwner(AActor* targetActor) { projectileOwner = targetActor; }
};
