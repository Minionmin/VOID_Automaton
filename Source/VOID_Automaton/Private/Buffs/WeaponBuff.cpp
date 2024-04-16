// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/WeaponBuff.h"

#include "Interfaces/HasWeapon.h"

UWeaponBuff::UWeaponBuff()
{
}

void UWeaponBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);

	// オーナーから武器レファレンスを取得
	if(equippedWeapon == nullptr)
	{
		// IHasWeaponインターフェースを持っているか確認する
		auto hasWeaponTarget = Cast<IHasWeapon>(target);
		if(hasWeaponTarget)
		{
			// 武器のレファレンスを取得する
			equippedWeapon = hasWeaponTarget->GetCurrentWeapon_Implementation();
		}
	}
}

void UWeaponBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);
}
