// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeBuff.h"
#include "StaminaRegenAmountBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UStaminaRegenAmountBuff : public UAttributeBuff
{
	GENERATED_BODY()

public:
	
	UStaminaRegenAmountBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
};
