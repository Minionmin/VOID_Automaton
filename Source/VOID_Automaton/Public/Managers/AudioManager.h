// Fill out your copyright notice in the Description page of Project Settings.

// シングルトンパターンを使用して、ゲーム内のオーディオを管理するクラス
// GameInstanceで初期化される

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AudioManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOID_AUTOMATON_API UAudioManager : public UObject
{
	GENERATED_BODY()

public:

	UAudioManager();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	float GetMasterVolume() const;
	UFUNCTION(BlueprintCallable, Category = "Audio")
	float GetBGMVolume() const;
	UFUNCTION(BlueprintCallable, Category = "Audio")
	float GetSFXVolume() const;
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetMasterVolume(float volume);
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetBGMVolume(float volume);
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetSFXVolume(float volume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	UAudioComponent* PlayBGM(USoundBase* soundBase, UObject* worldContextObject);
	UFUNCTION(BlueprintCallable, Category = "Audio")
	UAudioComponent* PlaySFX(USoundBase* soundBase, UObject* worldContextObject);

private:

	// 音量
	UPROPERTY()
	float masterVolume = 1.f;
	UPROPERTY()
	float bgmVolume = 1.f;
	UPROPERTY()
	float sfxVolume = 1.f;

	// 現在再生中の音声
	UPROPERTY()
	UAudioComponent* currentBGM = nullptr;
	UPROPERTY()
	UAudioComponent* currentSFX = nullptr;
};
