// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoValueChanged, int32, newAmmoValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadProgressStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadProgressChanged, float, newReloadProgress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadProgressFinished);

class UBoxComponent;

UCLASS()
class VOID_AUTOMATON_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();
	virtual void Tick(float DeltaTime) override;

	/*
		イベント
	*/
	FOnAmmoValueChanged OnAmmoValueChanged;
	FOnReloadProgressStarted OnReloadProgressStarted;
	FOnReloadProgressChanged OnReloadProgressChanged;
	FOnReloadProgressFinished OnReloadProgressFinished;

	UFUNCTION()
	void SetAmmo(int32 newAmmoVal);

	// ** リロード **
	UFUNCTION()
	void StartReloading();
	UFUNCTION()
	void StopReloading();

	// ** ゲッター **
	UFUNCTION()
	void SetCurrentAmmo(int32 newAmmo);
	UFUNCTION()
	void SetMaxAmmo(int32 newMaxAmmo);
	UFUNCTION()
	void SetTimeToReload(float newTimeToReload);
	UFUNCTION()
	void SetBodyshotDamage(float newDamage);
	UFUNCTION()
	void SetHeadshotDamage(float newHeadshotDamage);
	UFUNCTION()
	void SetTimeBetweenShot(float newTimeBetweenShot);
	UFUNCTION()
	void SetBulletRange(float newBulletRange);

	// 一般
	UFUNCTION(BlueprintCallable)
	void HideWeapon();
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void ProgressReload();
	float GetReloadProgressPercent() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* weaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* boxCollision;

	/*
		武器の属性
	*/
	// ** 弾 **
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Attributes | Ammo")
	int32 ammo = 0; // 現在の弾数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Ammo")
	int32 maxAmmo = 20;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Attributes | Ammo")
	float currentReloadProgress = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Ammo")
	float timeToReload = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Ammo")
	float baseTimeToReload = 1.f;
	FTimerHandle timerHandlerReloading;

	// ** ダメージ **
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Damage")
	float bodyDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Damage")
	float headshotDamage;

	// ** 射撃 **
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Fire Rate")
	float timeBetweenShot = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon | Attributes | Fire Range")
	float bulletRange = 4000.f;
public:

	FORCEINLINE int32 GetCurrentAmmo() const { return ammo; }
	FORCEINLINE int32 GetMaxAmmo() const { return maxAmmo; }
	FORCEINLINE float GetBaseTimeToReload() const { return baseTimeToReload; }
	FORCEINLINE float GetTimeToReload() const { return timeToReload; }
	FORCEINLINE float GetBodyshotDamage() const { return bodyDamage; }
	FORCEINLINE float GetHeadshotDamage() const { return headshotDamage; }
	FORCEINLINE float GetTimeBetweenShot() const { return timeBetweenShot; }
	FORCEINLINE float GetBulletRange() const { return bulletRange; }

	
	FORCEINLINE bool AmmoIsEmpty() const { return ammo == 0; }
	FORCEINLINE bool AmmoIsFull() const { return ammo == maxAmmo; }
};
