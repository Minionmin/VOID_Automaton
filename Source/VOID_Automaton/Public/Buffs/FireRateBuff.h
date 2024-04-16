// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buffs/WeaponBuff.h"
#include "FireRateBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UFireRateBuff : public UWeaponBuff
{
	GENERATED_BODY()
	
public:
	
	UFireRateBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
};
