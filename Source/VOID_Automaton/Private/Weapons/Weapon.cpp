// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Components/BoxComponent.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	SetRootComponent(weaponMesh);
	
	boxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	boxCollision->SetupAttachment(weaponMesh);
	
	baseTimeToReload = timeToReload;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// 初期弾数を設定する
	SetAmmo(maxAmmo);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SetAmmo(int32 newAmmoVal)
{
	ammo = newAmmoVal;
	OnAmmoValueChanged.Broadcast(ammo);
}

void AWeapon::StartReloading()
{
	// リロードのWidgetを表示する
	OnReloadProgressStarted.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(timerHandlerReloading, this, &AWeapon::ProgressReload, 0.01f, true);
}

void AWeapon::ProgressReload()
{
	// リロードの進行度を0.1ずつ増やす。0未満にはならず、最大リロード時間を超えないようにする
	currentReloadProgress = FMath::Clamp(currentReloadProgress + 0.01f, 0.0f, timeToReload);
	
	// リロードの進行度をパーセントでBroadcastする (リロードのWidgetに使う)
	OnReloadProgressChanged.Broadcast(GetReloadProgressPercent());

	// リロードが完了したらリロード行動を止める
	if(currentReloadProgress == timeToReload)
	{
		StopReloading();
	}
}

void AWeapon::StopReloading()
{
	// 任意の変数をリセットする
	GetWorld()->GetTimerManager().ClearTimer(timerHandlerReloading);
	currentReloadProgress = 0.f;
	SetAmmo(GetMaxAmmo());
	
	// リロードのWidgetを非表示にする
	OnReloadProgressFinished.Broadcast();
}

void AWeapon::SetCurrentAmmo(int32 newAmmo)
{
	ammo = FMath::Clamp(newAmmo, 0, maxAmmo);
	
	// UIに現在の弾数をBroadcastする
	OnAmmoValueChanged.Broadcast(ammo);
}

void AWeapon::SetMaxAmmo(int32 newMaxAmmo)
{

	maxAmmo = FMath::Clamp(newMaxAmmo, 1, 100);
}

void AWeapon::SetBodyshotDamage(float newDamage)
{
	bodyDamage = FMath::Clamp(newDamage, 1.f, TNumericLimits<int32>::Max());
}

void AWeapon::SetHeadshotDamage(float newHeadshotDamage)
{
	headshotDamage = FMath::Clamp(newHeadshotDamage, 1.f, TNumericLimits<int32>::Max());
}

void AWeapon::SetTimeToReload(float newTimeToReload)
{
	timeToReload = FMath::Clamp(newTimeToReload, 0.1f, 10.0);
}

void AWeapon::SetTimeBetweenShot(float newTimeBetweenShot)
{
	timeBetweenShot = FMath::Clamp(newTimeBetweenShot, 0.01f, 5.0f);
}

void AWeapon::SetBulletRange(float newBulletRange)
{
	bulletRange = FMath::Clamp(newBulletRange, 500.f, 20000);
}

float AWeapon::GetReloadProgressPercent() const
{
	return currentReloadProgress / timeToReload;
}

void AWeapon::HideWeapon()
{
	weaponMesh->SetVisibility(false);
}