// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

#include "VOID_Automaton/DebugMacros.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeStats();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// スタミナの管理
	TickStamina(DeltaTime);
}

void UAttributeComponent::TickStamina(float DeltaTime)
{
	// スタミナを所持していないキャラクターであれば何もしない
	// スタミナが回復中であれば何もしない
	if(!bHaveStamina || bIsRegeneratingStamina) return;

	// クールタイムが終わったらスタミナ回復を開始
	if(currentCooldownTillStaminaRegen <= 0.f)
	{
		StartRegeneratingStamina(staminaRegenAmount);
	}
	// スタミナが満タンでない時にクールタイムを進ませる
	else if(!StaminaIsFull())
	{
		currentCooldownTillStaminaRegen -= DeltaTime;
	}
}

void UAttributeComponent::InitializeStats()
{
	// 体力とスタミナの初期値を設定
	SetHealth(maxHealth);
	SetStamina(maxStamina);
	currentCooldownTillStaminaRegen = cooldownTillStaminaRegen;
}

void UAttributeComponent::ReceiveDamage(float damageAmount)
{
	SetHealth(health - damageAmount);
}

void UAttributeComponent::SetHealth(float newHealthValue)
{
	health = FMath::Clamp(newHealthValue, 0.f, maxHealth);
	OnHealthValueChanged.Broadcast(GetHealthPercentage()); // 更新された値をブロードキャスト
}

void UAttributeComponent::SetMaxHealth(float newMaxHealth)
{
	maxHealth = newMaxHealth;
	OnMaxHealthValueChanged.Broadcast(maxHealth); // 更新された値をブロードキャスト
}

void UAttributeComponent::SetStamina(float newStaminaValue)
{
	stamina = FMath::Clamp(newStaminaValue, 0.f, maxStamina);
	OnStaminaValueChanged.Broadcast(GetStaminaPercentage()); // 更新された値をブロードキャスト
}

void UAttributeComponent::SetMaxStamina(float newMaxStamina)
{
	maxStamina = newMaxStamina;
	OnMaxStaminaValueChanged.Broadcast(maxStamina); // 更新された値をブロードキャスト
}

void UAttributeComponent::SetStaminaRegenAmount(float newStaminaRegenAmount)
{
	staminaRegenAmount = FMath::Clamp(newStaminaRegenAmount, 0.1f, maxStamina); // スタミナの範囲を制限
	
	// スタミナが現在回復中であれば、新しい回復量でタイマーを再起動します
	RestartStaminaRegeneration();
}

void UAttributeComponent::SetStaminaRegenRate(float newStaminaRegenRate)
{
	staminaRegenRate = FMath::Clamp(newStaminaRegenRate, 0.1f, 10.f); // スタミナ回復量の範囲を制限
	
	// スタミナが現在回復中であれば、新しいレートでタイマーを再起動します
	RestartStaminaRegeneration();
}

void UAttributeComponent::SetCooldownTillStaminaRegen(float newCooldownTillStaminaRegen)
{
	cooldownTillStaminaRegen = FMath::Clamp(newCooldownTillStaminaRegen, 0.1f, 10.f); // スタミナ回復クールダウンの範囲を制限

	// スタミナが現在回復中でない場合、新しい時間でクールダウンを再開します
	if(!bIsRegeneratingStamina)
	{
		currentCooldownTillStaminaRegen = cooldownTillStaminaRegen;
	}
}

void UAttributeComponent::StartConsumingStamina(float consumeAmount, float consumeRate)
{
	bIsConsumingStamina = true;

	// 走っている中、毎秒スタミナを消費する
	FTimerDelegate staminaConsumptionDelegate;
	staminaConsumptionDelegate.BindUFunction(this, FName("ConsumeStamina"), consumeAmount);
	GetWorld()->GetTimerManager().SetTimer(timerHandlerStaminaConsumption, staminaConsumptionDelegate, consumeRate, true);
}

void UAttributeComponent::ConsumeStamina(float consumeAmount)
{
	SetStamina(stamina - consumeAmount);
	
	// スタミナが消費されるたびに回復クールダウンをリセット
	StopRegeneratingStamina();

	// スタミナが尽きた時にサブスクライバーに何をすべきか尋ねる
	if(HasNoStamina())
	{
		StopConsumingStamina();
		OnOutOfStamina.Broadcast();
	}
}

void UAttributeComponent::StopConsumingStamina()
{
	bIsConsumingStamina = false;
	GetWorld()->GetTimerManager().ClearTimer(timerHandlerStaminaConsumption);
}

void UAttributeComponent::StartRegeneratingStamina(float regenAmount)
{
	bIsRegeneratingStamina = true;
	FTimerDelegate staminaRegenDelegate;
	staminaRegenDelegate.BindUFunction(this, FName("RegenerateStamina"), regenAmount);
	GetWorld()->GetTimerManager().SetTimer(timerHandlerStaminaRegen, staminaRegenDelegate, staminaRegenRate, true);
}

void UAttributeComponent::RegenerateStamina(float regenAmount)
{
	SetStamina(stamina + regenAmount);
	
	// スタミナが満タンになったらスタミナ回復を停止
	if(StaminaIsFull())
	{
		StopRegeneratingStamina();
	}
}

void UAttributeComponent::StopRegeneratingStamina()
{
	// スタミナ回復が停止された時にクールダウンをリセット
	currentCooldownTillStaminaRegen = cooldownTillStaminaRegen;
	bIsRegeneratingStamina = false;
	GetWorld()->GetTimerManager().ClearTimer(timerHandlerStaminaRegen);
}

void UAttributeComponent::RestartStaminaRegeneration()
{
	if(bIsRegeneratingStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandlerStaminaRegen);
		StartRegeneratingStamina(staminaRegenAmount);
	}
}