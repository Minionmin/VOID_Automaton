// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OptionBaseWidget.h"
#include "GameplayOptionWidget.generated.h"

class USlider;
class UTextBlock;
class UPlayerInputManagerComponent;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UGameplayOptionWidget : public UOptionBaseWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	virtual void ApplySetting() override;
	virtual void RevertSetting() override;

protected:

	virtual void InitializeSetting() override;

	UFUNCTION()
	void OnMouseHipFireValueChanged(float value);
	UFUNCTION()
	void OnMouseAimValueChanged(float value);
	UFUNCTION()
	void OnControllerHipFireValueChanged(float value);
	UFUNCTION()
	void OnControllerAimValueChanged(float value);
	
private:

	// スライダー設定
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* mouseHipFireSlider;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* mouseAimSlider;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* controllerHipFireSlider;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	USlider* controllerAimSlider;
	
	// テキスト
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* mouseHipFireText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* mouseAimText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* controllerHipFireText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* controllerAimText;

	UPROPERTY(VisibleAnywhere)
	UPlayerInputManagerComponent* inputManagerComponent = nullptr;
};
