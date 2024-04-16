// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/DamageBuff.h"

#include "Interfaces/HasWeapon.h"
#include "Weapons/Weapon.h"

UDamageBuff::UDamageBuff()
{
}

void UDamageBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);
	
	if(equippedWeapon)
	{
		// 身体に対するダメージを増加する
		const auto currentDamage = equippedWeapon->GetBodyshotDamage();
		equippedWeapon->SetBodyshotDamage(currentDamage + buffStrength);
	
		// ヘッドショットに対するダメージを増加する
		const auto currentHeadshotDamage = equippedWeapon->GetHeadshotDamage();
		equippedWeapon->SetHeadshotDamage(currentHeadshotDamage + buffStrength);
	}
}

void UDamageBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);

	if(equippedWeapon)
	{
		// 身体に対するダメージを元に戻す
		const auto currentDamage = equippedWeapon->GetBodyshotDamage();
		equippedWeapon->SetBodyshotDamage(currentDamage - buffStrength);
	
		// ヘッドショットに対するダメージを元に戻す
		const auto currentHeadshotDamage = equippedWeapon->GetHeadshotDamage();
		equippedWeapon->SetHeadshotDamage(currentHeadshotDamage - buffStrength);
	}
}
