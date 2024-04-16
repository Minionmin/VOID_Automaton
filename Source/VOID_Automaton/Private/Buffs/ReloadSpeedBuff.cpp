// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/ReloadSpeedBuff.h"

#include "Interfaces/HasWeapon.h"
#include "Weapons/Weapon.h"

UReloadSpeedBuff::UReloadSpeedBuff()
{
}

void UReloadSpeedBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(equippedWeapon)
	{
		// リロード速度を増加する
		const auto currentReloadSpeed = equippedWeapon->GetTimeToReload();
		equippedWeapon->SetTimeToReload(currentReloadSpeed - buffStrength);
	}
}

void UReloadSpeedBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);

	if(equippedWeapon)
	{
		// リロード速度を元に戻す
		const auto currentReloadSpeed = equippedWeapon->GetTimeToReload();
		equippedWeapon->SetTimeToReload(currentReloadSpeed + buffStrength);
	}
}
