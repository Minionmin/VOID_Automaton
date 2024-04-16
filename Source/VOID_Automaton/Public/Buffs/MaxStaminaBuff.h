// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buffs/AttributeBuff.h"
#include "MaxStaminaBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMaxStaminaBuff : public UAttributeBuff
{
	GENERATED_BODY()
	
public:
	UMaxStaminaBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
};
