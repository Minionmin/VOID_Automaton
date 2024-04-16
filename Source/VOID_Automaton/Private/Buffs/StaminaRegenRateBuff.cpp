// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Buffs\StaminaRegenRateBuff.h"

#include "Components/AttributeComponent.h"

UStaminaRegenRateBuff::UStaminaRegenRateBuff()
{
	
}

void UStaminaRegenRateBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ回復間隔を短縮する
		const auto currentStaminaRegenRate = attributeComponent->GetStaminaRegenRate();
		attributeComponent->SetStaminaRegenRate(currentStaminaRegenRate - buffStrength);	
	}
}

void UStaminaRegenRateBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ回復間隔を元に戻す
		const auto currentStaminaRegenRate = attributeComponent->GetStaminaRegenRate();
		attributeComponent->SetStaminaRegenAmount(currentStaminaRegenRate + buffStrength);	
	}
}
