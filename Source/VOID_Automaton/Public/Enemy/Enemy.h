// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hitable.h"
#include "../Plugins/CustomStateMachine/Source/CustomStateMachine/Public/StateManagerComponent.h"
#include "Enemy.generated.h"

class USoundBase;
class UParticleSystem;
class UAttributeComponent;
class UWidgetComponent;
class AEnemyAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakingDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamagedPlayer);

UCLASS()
class VOID_AUTOMATON_API AEnemy : public ACharacter, public IHitable
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(FHitResult hitResult) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/*
		Events
	*/
	FOnEnemyDead OnEnemyDead;
	FOnTakingDamage OnTakingDamage;
	FOnDamagedPlayer OnDamagedPlayer;
	
	/*
		UIs
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void ShowDmgIndicator(int32 takenDamage, FVector hitLocation, bool isHeadShotDmg);
	UFUNCTION(BlueprintCallable)
	void StoreDmgIndicator(UUserWidget* dmgIndicatorWidget, FVector location);
	
protected:

	virtual void BeginPlay() override;

	/*
		Combat
	*/
	virtual void Die();
	
	/*
		UIs | Damage Indicator
	*/
	UFUNCTION()
	void DestroyDmgIndicator(UUserWidget* dmgIndicatorWidgetToDestroy);
	UFUNCTION()
	void UpdateDmgIndicator(); // Update dmg indicator location every frame

public:
	/*
		State Machine
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Machine")
	UStateManagerComponent* stateManager = nullptr;
	
protected:

	/*
		Components
	*/
	// ** Animation ** //
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimInstance* animInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General")
	FString headBone;
	UPROPERTY(VisibleAnywhere, Category = "General")
	AEnemyAIController* enemyAIController = nullptr;
	
	// ** UI ** //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> enemyHealthBarWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UEnemyHealthBar* enemyHealthBarWidget;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TMap<UUserWidget*, FVector> dmgIndicators; // Map to store dmgIndicator widgets and their location
	UPROPERTY(EditAnywhere, Category = "UI")
	float dmgIndicatorDestroyTime = 0.5f; // Timer before removing a dmgIndicator from player screen
	
	/*
		Stats
	*/
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attributes;
	UPROPERTY(EditAnywhere, Category = "General")
	FString enemyName = "default";

	// ** Movement ** //
	UPROPERTY(EditDefaultsOnly)
	float turnRate = 4.f;

	// ** State ** //
	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsDead = false;
	
	/*
		Combat
	*/
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TArray<AActor*> hitActors;
	
	/*
		VFXs
	*/
	UPROPERTY(EditDefaultsOnly, Category = "VFX | Combat")
	UParticleSystem* impactParticleVFX;
	
	/*
		SFXs
	*/
	UPROPERTY(EditDefaultsOnly, Category = "SFX | Combat")
	USoundBase* impactSFX;

public:

	UFUNCTION()
	UAttributeComponent* GetAttribute() const { return attributes; }
	UFUNCTION()
	UAnimInstance* GetEnemyAnimInstance() const { return animInstance; }
	FORCEINLINE FString GetHeadBone() const { return headBone; }
	FORCEINLINE FString GetEnemyName() const { return enemyName; }
	FORCEINLINE float GetTurnRate() const { return turnRate; }
	FORCEINLINE AEnemyAIController* GetAIController() const { return enemyAIController; }
	FORCEINLINE bool IsDead() const { return bIsDead; }
};
