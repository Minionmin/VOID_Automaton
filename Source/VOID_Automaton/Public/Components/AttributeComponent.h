
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaValueChanged, float, newStaminaValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthValueChanged, float, newHealthValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthValueChanged, float, newMaxHealthValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaValueChanged, float, newMaxStaminaValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOID_AUTOMATON_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
		イベント
	*/
	FOnOutOfStamina OnOutOfStamina;
	FOnHealthValueChanged OnHealthValueChanged;
	FOnStaminaValueChanged OnStaminaValueChanged;
	FOnMaxHealthValueChanged OnMaxHealthValueChanged;
	FOnMaxStaminaValueChanged OnMaxStaminaValueChanged;
	
	// ** 体力 **
	UFUNCTION()
	void ReceiveDamage(float damageAmount);

	// ** スタミナ **
	UFUNCTION()
	void StartConsumingStamina(float consumeAmount, float consumeRate);
	UFUNCTION()
	void ConsumeStamina(float consumeAmount);
	UFUNCTION()
	void StopConsumingStamina();
	UFUNCTION()
	void StartRegeneratingStamina(float regenAmount);
	UFUNCTION()
	void RegenerateStamina(float regenAmount);
	UFUNCTION()
	void StopRegeneratingStamina();
	UFUNCTION()
	void RestartStaminaRegeneration();

	/*
		セッター
	*/
	UFUNCTION()
	void SetHealth(float newHealthValue);
	UFUNCTION()
	void SetStamina(float newStaminaValue);
	UFUNCTION()
	void SetMaxHealth(float newMaxHealth);
	UFUNCTION()
	void SetMaxStamina(float newMaxStamina);
	UFUNCTION()
	void SetStaminaRegenAmount(float newStaminaRegenAmount);
	UFUNCTION()
	void SetStaminaRegenRate(float newStaminaRegenRate);
	UFUNCTION()
	void SetCooldownTillStaminaRegen(float newCooldownTillStaminaRegen);
	
protected:

	virtual void BeginPlay() override;
	virtual void InitializeStats();
	void TickStamina(float DeltaTime);
	
protected:

	// ** 体力 **
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Health")
	float health = 0.f; // current health
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Health")
	float maxHealth = 0.f;

	// ** スタミナ **
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina")
	bool bHaveStamina = false; // このキャラクターがスタミナを持っているかどうか（エディターで設定可能）
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float stamina = 0.f; // 現在のスタミナ
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float maxStamina = 0.f;

	// ** スタミナ | 消費 **
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float runningStaminaCost = 4.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float runningStaminaConsumeRate = 0.2f; // consuming interval
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float dashingStaminaCost = 10.f;
	FTimerHandle timerHandlerStaminaConsumption;
	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	bool bIsConsumingStamina = false;
	
	// ** スタミナ | 回復 **
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float staminaRegenAmount = 5.f;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float staminaRegenRate = 1.f; // 回復間隔
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float cooldownTillStaminaRegen = 1.5f; // スタミナ回復が始まるまでのクールダウン
	UPROPERTY(EditAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	float currentCooldownTillStaminaRegen = 1.5f; // 現在スタミナ回復が始まるまでのクールダウン
	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes | Stamina", meta = (Editcondition = "bHaveStamina", EditConditionHides = "bHaveStamina"))
	bool bIsRegeneratingStamina = false;
	FTimerHandle timerHandlerStaminaRegen;

public:

	// ** ゲッター **
	FORCEINLINE float GetCurrentHealth() const { return health; }
	FORCEINLINE float GetMaxHealth() const { return maxHealth; }
	FORCEINLINE float GetHealthPercentage() const { return health / maxHealth; }
	FORCEINLINE float GetCurrentStamina() const { return stamina; }
	FORCEINLINE float GetMaxStamina() const { return maxStamina; }
	FORCEINLINE float GetStaminaPercentage() const { return stamina / maxStamina; }
	FORCEINLINE float GetStaminaRegenAmount () const { return staminaRegenAmount; }
	FORCEINLINE float GetStaminaRegenRate() const { return staminaRegenRate; }
	FORCEINLINE float GetCooldownTillStaminaRegen() const { return cooldownTillStaminaRegen; }
	FORCEINLINE float GetRunningStaminaCost() const { return runningStaminaCost; }
	FORCEINLINE float GetRunningStaminaConsumeRate() const { return runningStaminaConsumeRate; }
	FORCEINLINE float GetDashingStaminaCost() const { return dashingStaminaCost; }
	
	// ** リソースのステート **
	FORCEINLINE bool StaminaIsFull() const { return stamina == maxStamina; }
	FORCEINLINE bool HasNoStamina() const { return stamina == 0.f; }
	FORCEINLINE bool HasNoHealth() const { return health == 0.f; }
};
