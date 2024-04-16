// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OptionBaseWidget.h"
#include "Blueprint/UserWidget.h"
#include "AudioOptionWidget.generated.h"

class USlider;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UAudioOptionWidget : public UOptionBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	virtual void ApplySetting() override;
	virtual void RevertSetting() override;

protected:

	virtual void InitializeSetting() override;

	UFUNCTION()
	void OnMasterValueChanged(float value);
	UFUNCTION()
	void OnBGMValueChanged(float value);
	UFUNCTION()
	void OnSFXValueChanged(float value);
	
private:

	// スライダー設定
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* masterSlider;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* bgmSlider;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* sfxSlider;

	// テキスト
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* masterText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* bgmText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* sfxText;

	// GameInstance
	UPROPERTY(VisibleAnywhere)
	class UMainGameInstance* gameInstance;
};
