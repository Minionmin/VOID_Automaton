// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OptionBaseWidget.h"
#include "GraphicsOptionWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UGraphicsOptionWidget : public UOptionBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void ApplySetting() override;
	virtual void RevertSetting() override;

protected:
	virtual void InitializeSetting() override;
	
private:
	
	UFUNCTION()
	void OnWindowModeButtonClicked();
	UFUNCTION()
	void OnResolutionButtonClicked();
	UFUNCTION()
	void OnGraphicButtonClicked();
	UFUNCTION()
	void OnVsyncButtonClicked();

	UFUNCTION()
	void SetWindowMode(int windowModeType);
	void SetWindowMode(EWindowMode::Type windowModeType);
	UFUNCTION()
	void SetResolution(int resolutionType);
	void SetResolution(FIntPoint screenResolution);
	UFUNCTION()
	void SetGraphics(int graphicType);
	
private:

	// ボタン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* windowModeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* resolutionButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* graphicButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* vsyncButton;
	
	// テキスト
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* windowModeText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* resolutionText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* graphicText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* vsyncText;

	// オプションインデックス
	UPROPERTY(VisibleAnywhere)
	int32 windowModeIndex = 0;
	UPROPERTY(VisibleAnywhere)
	int32 resolutionIndex = 0;
	UPROPERTY(VisibleAnywhere)
	int32 graphicIndex = 0;
	UPROPERTY(VisibleAnywhere)
	bool IsVsyncActive = true;

	// プレイヤーのゲーム設定レファレンス
	UGameUserSettings* gameUserSettings = nullptr;
};
