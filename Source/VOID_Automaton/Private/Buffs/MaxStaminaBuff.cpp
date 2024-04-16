// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/MaxStaminaBuff.h"

#include "Components/AttributeComponent.h"

UMaxStaminaBuff::UMaxStaminaBuff()
{
}

void UMaxStaminaBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(attributeComponent)
	{
		// スタミナ上限を増加する
		const auto currentMaxStamina = attributeComponent->GetMaxStamina();
		attributeComponent->SetMaxStamina(currentMaxStamina + buffStrength);
		attributeComponent->SetStamina(attributeComponent->GetMaxStamina());
	}
}

void UMaxStaminaBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);

	if(attributeComponent)
	{
		// スタミナ上限を元に戻す
		const auto currentMaxStamina = attributeComponent->GetMaxStamina();
		attributeComponent->SetMaxStamina(currentMaxStamina - buffStrength);
		attributeComponent->SetStamina(attributeComponent->GetMaxStamina());
	}
}
