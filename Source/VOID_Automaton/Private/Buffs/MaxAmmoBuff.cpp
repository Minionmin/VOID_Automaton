// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/MaxAmmoBuff.h"

#include "Weapons/Weapon.h"

UMaxAmmoBuff::UMaxAmmoBuff()
{
}

void UMaxAmmoBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);

	if(equippedWeapon)
	{
		// 最大弾数を増加する
		const auto currentMaxAmmo = equippedWeapon->GetMaxAmmo();
		equippedWeapon->SetMaxAmmo(currentMaxAmmo + static_cast<int32>(buffStrength));
		// 現在の弾数を最大弾数に合わせる
		equippedWeapon->SetCurrentAmmo(equippedWeapon->GetMaxAmmo());
	}
}

void UMaxAmmoBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);

	if(equippedWeapon)
	{
		// 最大弾数を元に戻す
		const auto currentMaxAmmo = equippedWeapon->GetMaxAmmo();
		equippedWeapon->SetMaxAmmo(currentMaxAmmo - static_cast<int32>(buffStrength));
		// 現在の弾数を最大弾数に合わせる
		equippedWeapon->SetCurrentAmmo(equippedWeapon->GetMaxAmmo());
	}
}
