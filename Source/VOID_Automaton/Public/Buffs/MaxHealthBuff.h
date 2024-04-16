// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buffs/AttributeBuff.h"
#include "MaxHealthBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMaxHealthBuff : public UAttributeBuff
{
	GENERATED_BODY()
	
public:
	
	UMaxHealthBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
};
