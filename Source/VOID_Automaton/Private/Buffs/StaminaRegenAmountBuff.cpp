// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/StaminaRegenAmountBuff.h"

#include "Components/AttributeComponent.h"

UStaminaRegenAmountBuff::UStaminaRegenAmountBuff()
{
}

void UStaminaRegenAmountBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ回復量を増加させる
		const auto currentStaminaRegenAmount = attributeComponent->GetStaminaRegenAmount();
		attributeComponent->SetStaminaRegenAmount(currentStaminaRegenAmount + buffStrength);	
	}
}

void UStaminaRegenAmountBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ回復量を元に戻す
		const auto currentStaminaRegenAmount = attributeComponent->GetStaminaRegenAmount();
		attributeComponent->SetStaminaRegenAmount(currentStaminaRegenAmount - buffStrength);
	}
}
