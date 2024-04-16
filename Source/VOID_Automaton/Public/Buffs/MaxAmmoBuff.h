// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buffs/WeaponBuff.h"
#include "MaxAmmoBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMaxAmmoBuff : public UWeaponBuff
{
	GENERATED_BODY()

public:

	UMaxAmmoBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
};
