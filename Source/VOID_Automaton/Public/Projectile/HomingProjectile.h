// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraProjectileBase.h"
#include "Projectile/ProjectileBase.h"
#include "HomingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API AHomingProjectile : public ANiagaraProjectileBase
{
	GENERATED_BODY()
	
public:	

	AHomingProjectile();
	virtual void Tick(float DeltaTime) override;

	virtual FGameplayTagContainer GetGameplayTagContainer_Implementation() override;
	
	/*
		Overlap Events
	*/
	virtual void OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	
	/*
		Homing Pattern
	*/
	UFUNCTION(BlueprintCallable)
	static void StartHomingRangeAttackFanShape(AActor* spawnedBy, AActor* targetActor, TSubclassOf<AHomingProjectile> projectileSubclass,
		int32 projectileCount, float fanRadius, float zOffset, float projectileSpeed, FRotator projectileRotation, bool bShouldSpawn = false);
	
protected:

	virtual void BeginPlay() override;

public:
	
	/*
		State Machine
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Machine")
	class UStateManagerComponent* stateManager = nullptr;

	/*
		In case we want to spawn other projectiles from this projectile 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern")
	bool bShouldSpawnProjectile = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern", meta = (EditCondition = "bShouldSpawnProjectile", EditConditionHides = "bShouldSpawnProjectile"))
	TSubclassOf<AHomingProjectile> homingProjectileClassToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern", meta = (EditCondition = "bShouldSpawnProjectile", EditConditionHides = "bShouldSpawnProjectile"))
	int32 projectileCount = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern", meta = (EditCondition = "bShouldSpawnProjectile", EditConditionHides = "bShouldSpawnProjectile"))
	float fanRadius = 180.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern", meta = (EditCondition = "bShouldSpawnProjectile", EditConditionHides = "bShouldSpawnProjectile"))
	float zOffset = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pattern", meta = (EditCondition = "bShouldSpawnProjectile", EditConditionHides = "bShouldSpawnProjectile"))
	float projectileSpeed = 50.f;
	
	
protected:

	/*
		State Configures
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State | Preparing")
	float maxPreparingTime = 2.f; // Called in homing projectile preparing state
	UPROPERTY(VisibleInstanceOnly)
	AActor* target = nullptr; // Homing target
	
public:

	UFUNCTION(BlueprintCallable)
	float GetProjectileMaxPreparingTime() const { return maxPreparingTime; }
	UFUNCTION(BlueprintCallable)
	AActor* GetProjectileTarget() const { return target; }
	UFUNCTION(BlueprintCallable)
	void SetProjectileTarget(AActor* targetActor) { target = targetActor; }
};
