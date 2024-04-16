// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/MaxHealthBuff.h"
#include "Components/AttributeComponent.h"
#include "Interfaces/HasAttribute.h"

UMaxHealthBuff::UMaxHealthBuff()
{
}

void UMaxHealthBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(attributeComponent)
	{
		// 体力上限を増加する
		const auto currentMaxHP = attributeComponent->GetMaxHealth();
		attributeComponent->SetMaxHealth(currentMaxHP + buffStrength);
		attributeComponent->SetHealth(attributeComponent->GetMaxHealth());
	}
}

void UMaxHealthBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);
	
	if(attributeComponent)
	{
		// 体力上限を元に戻す
		const auto currentMaxHP = attributeComponent->GetMaxHealth();
		attributeComponent->SetMaxHealth(currentMaxHP - buffStrength);
		attributeComponent->SetHealth(attributeComponent->GetMaxHealth());
	}
}
