// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenuWidget.generated.h"

class UOverlay;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// 設定タブボタンイベント
	UFUNCTION()
	void OnGraphicsButtonClicked();
	UFUNCTION()
	void OnGameplayButtonClicked();
	UFUNCTION()
	void OnAudioButtonClicked();
	UFUNCTION()
	void OnCloseButtonClicked();
	UFUNCTION()
	void OnApplyButtonClicked();
	UFUNCTION()
	void OnRevertButtonClicked();
	
	UFUNCTION()
	void CreateSubWidget(TSubclassOf<UUserWidget> widgetClass, UUserWidget*& widgetPointer, UOverlay* parentOverlay);
	UFUNCTION()
	void RemoveSubWidgets();
	UFUNCTION()
	void ApplySubWidgetSetting(UUserWidget* widget);
	UFUNCTION()
	void RevertSubWidgetSetting(UUserWidget* widget);
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* subOptionTitle;
	
	// 設定タブボタン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* graphicsButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* gameplayButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* audioButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* closeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* applyButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* revertButton;
	
	// サブウィジェットを格納する場所
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UOverlay* subWBPOverlay;

	// 項目ごとの設定ウィジェット　
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> graphicOptionWidgetClass;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> gameplayOptionWidgetClass;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> audioOptionWidgetClass;

	// サブウィジェットのインスタンス
	UPROPERTY(VisibleAnywhere)
	UUserWidget* graphicOptionWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	UUserWidget* gameplayOptionWidget = nullptr;
	UPROPERTY(VisibleAnywhere)
	UUserWidget* audioOptionWidget = nullptr;
};
