// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/StaminaRegenCooldownBuff.h"

#include "Components/AttributeComponent.h"

UStaminaRegenCooldownBuff::UStaminaRegenCooldownBuff()
{
}

void UStaminaRegenCooldownBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ回復までの時間を短縮する
		const auto currentStaminaRegenCooldown = attributeComponent->GetCooldownTillStaminaRegen();
		attributeComponent->SetCooldownTillStaminaRegen(currentStaminaRegenCooldown - buffStrength);	
	}
}

void UStaminaRegenCooldownBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ回復までの時間を元に戻す
		const auto currentStaminaRegenCooldown = attributeComponent->GetCooldownTillStaminaRegen();
		attributeComponent->SetCooldownTillStaminaRegen(currentStaminaRegenCooldown + buffStrength);	
	}
}
