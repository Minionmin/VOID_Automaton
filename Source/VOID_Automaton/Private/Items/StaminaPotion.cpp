// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/StaminaPotion.h"

#include "VOIDCharacter.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

UStaminaPotion::UStaminaPotion()
{
}

void UStaminaPotion::Interact_Implementation(AVOIDCharacter* character)
{
	const auto attributeComponent = character->GetAttribute_Implementation();
	if(attributeComponent)
	{
		attributeComponent->SetStamina(attributeComponent->GetCurrentStamina() + potionStrength);	
	}

	if(potionParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(potionParticle, character->GetMesh());	
	}
}
