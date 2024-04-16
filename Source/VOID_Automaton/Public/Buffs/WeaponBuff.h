// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buffs/Buff.h"
#include "WeaponBuff.generated.h"

class AWeapon;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UWeaponBuff : public UBuff
{
	GENERATED_BODY()

public:

	UWeaponBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Buff")
	AWeapon* equippedWeapon;
	
};
