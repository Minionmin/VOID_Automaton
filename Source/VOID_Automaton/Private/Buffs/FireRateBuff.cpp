// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/FireRateBuff.h"

#include "Interfaces/HasWeapon.h"
#include "Weapons/Weapon.h"

UFireRateBuff::UFireRateBuff()
{
}

void UFireRateBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(equippedWeapon)
	{
		// 射撃速度を増加する
		const auto currentFireRate = equippedWeapon->GetTimeBetweenShot();
		equippedWeapon->SetTimeBetweenShot(currentFireRate - buffStrength);
	}
}

void UFireRateBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);

	if(equippedWeapon)
	{
		// 射撃速度を元に戻す
		const auto currentFireRate = equippedWeapon->GetTimeBetweenShot();
		equippedWeapon->SetTimeBetweenShot(currentFireRate + buffStrength);
	}
}
