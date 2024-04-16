// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeBuff.h"
#include "StaminaRegenRateBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UStaminaRegenRateBuff : public UAttributeBuff
{
	GENERATED_BODY()
	
public:
	
	UStaminaRegenRateBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
};
