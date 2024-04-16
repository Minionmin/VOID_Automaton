// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Navigation/PathFollowingComponent.h"
#include "Projectile/ProjectileBase.h"
#include "Witch.generated.h"

class ACascadeProjectileBase;
class AHomingProjectile;
class UBoxComponent;
class ATargetPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportEnd, FVector, endLocation);

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API AWitch : public AEnemy
{
	GENERATED_BODY()
	
public:

	AWitch();

	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(FHitResult hitResult) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/*
		Delegates
	*/
	FOnTeleportEnd OnTeleportEnd;
	
	/*
		Hitbox overlap events
	*/
	UFUNCTION()
	virtual void OnScreamAttackBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	virtual void OnMeleeAttackBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
	/*
		Moving Patterns
	*/
	UFUNCTION(BlueprintCallable)
	void NormalTeleport(const FVector& destination);
	UFUNCTION(BlueprintCallable)
	void CombatTeleport(const FVector& destination);
	
	// Without delegate
	void InitiateMoveTo(const FVector& targetLocation, const float acceptableRadius);
	// With delegate
	void InitiateMoveTo(const FVector& targetLocation, const float acceptableRadius,
		const FScriptDelegate& movementCompleteDelegate, FPathFollowingRequestResult &outResult);
	
	/*
		Attack Patterns
	*/
	UFUNCTION(BlueprintCallable)
	void SpawnSwipeProjectile();
	UFUNCTION(BlueprintCallable)
	void SpawnHomingRangeProjectile();
	UFUNCTION(BlueprintCallable)
	void SpawnNormalProjectile();
	UFUNCTION(BlueprintCallable)
    void ActivateHitbox(UBoxComponent* hitbox);
	UFUNCTION(BlueprintCallable)
	void DeactivateHitbox(UBoxComponent* hitbox);

protected:

	virtual void BeginPlay() override;

	// ** Combat **
	virtual void Die() override;
	
	UFUNCTION()
	void Teleport(const FVector& destination);
	UFUNCTION()
	void TeleportEnd();
	UFUNCTION()
	void SpawnCombatTeleportEffectAt(FVector location);
	UFUNCTION()
	void SpawnNormalTeleportEffectAt(FVector location);
	
public:
	
	/*
		Stat
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float meleeAttackDamage = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float screamAttackDamage = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float screamKnockbackDistance = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float screamKnockbackZDistance = 50.f;
	
	/*
		State Configures
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	bool bhasEnteredSecondPhase = false;

	// ** General projectile **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "50", ClampMax = "500", UIMin = "50", UIMax = "500"))
	float projectileDirectionOffset = 200.f;
	
	// ** Swipe projectile **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Swipe Projectile", meta = (ClampMin = "50", ClampMax = "500", UIMin = "50", UIMax = "500"))
	float centerOffset = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Swipe Projectile", meta = (ClampMin = "100", ClampMax = "5000", UIMin = "100", UIMax = "5000"))
	float swipeProjectileSpeed = 1500.f;
	
	// ** Range attack 1 (Normal) **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 1", meta = (ClampMin = "0.1", ClampMax = "10", UIMin = "0.1", UIMax = "10"))
	float projectile1CastTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 1", meta = (ClampMin = "1", ClampMax = "11", UIMin = "1", UIMax = "11"))
	int32 projectile1Count = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 1", meta = (ClampMin = "150", ClampMax = "300", UIMin = "150", UIMax = "300"))
	float projectile1ZOffset = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 1", meta = (ClampMin = "100", ClampMax = "5000", UIMin = "100", UIMax = "5000"))
	float projectile1Speed = 2000.f;
	
	// ** Range attack 2 (Homing) **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 2", meta = (ClampMin = "0.1", ClampMax = "10", UIMin = "0.1", UIMax = "10"))
	float homingCastTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 2", meta = (ClampMin = "1", ClampMax = "11", UIMin = "1", UIMax = "11"))
	int32 homingProjectileCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 2", meta = (ClampMin = "45", ClampMax = "270", UIMin = "45", UIMax = "270"))
	float homingFanRadius = 180.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 2", meta = (ClampMin = "150", ClampMax = "300", UIMin = "150", UIMax = "300"))
	float homingZOffset = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Range Attack 2", meta = (ClampMin = "100", ClampMax = "2000", UIMin = "100", UIMax = "2000"))
	float homingProjectileSpeed = 100.f;
	
	
	/*
		Animmontage
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animmontage")
	UAnimMontage* castingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animmontage")
	UAnimMontage* deathMontage;

	/*
		VFX
	*/
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* teleportVFX;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* combatTeleportVFX;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* meleeVFX;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* secondPhaseVFX;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* deathVFX;
	// deathVFX scale
	UPROPERTY(EditAnywhere, Category = "VFX")
	FVector deathVFXScale = FVector(1.f);
	
	/*
		SFX
	*/
	// ** SFX | Hide **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX | Hiding")
	USoundBase* whisperingNormalSFX;

	// ** SFX | Combo **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX | Hiding")
	USoundBase* laughingSFX;
	
	// ** SFX | Casting **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX | Casting")
	USoundBase* castingMurmurSFX;
	
	// ** SFX | Others **
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX | Others")
	USoundBase* screechSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX | Others")
	USoundBase* secondPhaseScreechSFX;
	
private:

	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* swipeProjectileSpawnLocation;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* meleeAttackBoxCollision;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* screamAttackBoxCollision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> screamCamShakeClass;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> meleeCamShakeClass;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> deathCameraShakeClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* movingTrailVFXComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* rightPunchVFXComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* leftPunchVFXComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* rightFootVFXComponent;

	/*
		Movement
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float originalTeleportVelocity = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float teleportVelocity = 5000.f;
	
	/*
		Combat Blueprint
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACascadeProjectileBase> swipeProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> normalProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHomingProjectile> homingProjectileClass;
	
	/*
		State
	*/
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsAttacking = false;
	UPROPERTY(VisibleAnywhere, Category = "State")
    bool bIsLeaping = false;
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsEvading = false;
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsScreaming = false;

public:
	
	FORCEINLINE bool ShouldSpawnSecondWave() const { return bhasEnteredSecondPhase; }
	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetMeleeAttackHitbox() const { return meleeAttackBoxCollision; }
	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetScreamAttackHitbox() const { return screamAttackBoxCollision; }
	UFUNCTION(BlueprintCallable)
	void SetAttacking(bool bState) { bIsAttacking = bState; }
	UFUNCTION(BlueprintCallable)
	void SetLeaping(bool bState) { bIsLeaping = bState; }
	UFUNCTION(BlueprintCallable)
	void SetEvading(bool bState) { bIsEvading = bState; }
	UFUNCTION(BlueprintCallable)
	void SetScreaming(bool bState) { bIsScreaming = bState; }
	UFUNCTION(BlueprintCallable)
	float GetOriginalTeleportVelocity() { return originalTeleportVelocity; }
	UFUNCTION(BlueprintCallable)
	void SetTeleportVelocity(float newVelocity) { teleportVelocity = newVelocity; }
	UFUNCTION(BlueprintCallable)
	bool IsLeaping() const { return bIsLeaping; }
	UFUNCTION(BlueprintCallable)
	bool IsEvading() const { return bIsEvading; }
	UFUNCTION(BlueprintCallable)
	bool IsScreaming() const { return bIsScreaming; }
	UFUNCTION(BlueprintCallable)
	void EnableTrailVFX();
	UFUNCTION(BlueprintCallable)
	void DisableTrailVFX();
};
