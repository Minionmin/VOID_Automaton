// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSaveStruct.generated.h"

class AWeapon;
/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct VOID_AUTOMATON_API FPlayerSaveStruct : public FTableRowBase
{
	GENERATED_BODY()

	// バフのセーブデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> buffSaveDataIDs;
	
	/*
		位置情報
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector playerLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator controllerRotation = FRotator::ZeroRotator;
	
	/*
		設定
	*/
	// ** 感度 ** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mouseHipTurnRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mouseAimTurnRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float controllerHipTurnRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float controllerAimTurnRate = 1.0f;

	// ** 音量 ** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float masterVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bgmVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sfxVolume = 1.0f;
};
