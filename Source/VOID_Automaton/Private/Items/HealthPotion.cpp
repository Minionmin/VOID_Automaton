// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthPotion.h"

#include "VOIDCharacter.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

UHealthPotion::UHealthPotion()
{
}

void UHealthPotion::Interact_Implementation(AVOIDCharacter* character)
{
	const auto attributeComponent = character->GetAttribute_Implementation();
	if(attributeComponent)
	{
		attributeComponent->SetHealth(attributeComponent->GetCurrentHealth() + potionStrength);	
	}
	
	if(potionParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(potionParticle, character->GetMesh());	
	}
}
