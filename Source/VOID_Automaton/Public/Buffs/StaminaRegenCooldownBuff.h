// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeBuff.h"
#include "Buffs/Buff.h"
#include "StaminaRegenCooldownBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UStaminaRegenCooldownBuff : public UAttributeBuff
{
	GENERATED_BODY()
	
public:
	
	UStaminaRegenCooldownBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
	
};
