// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Potion.h"
#include "StaminaPotion.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOID_AUTOMATON_API UStaminaPotion : public UPotion
{
	GENERATED_BODY()

public:

	UStaminaPotion();

	virtual void Interact_Implementation(class AVOIDCharacter* character) override;
};
