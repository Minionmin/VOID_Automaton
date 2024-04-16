// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/OptionMenuWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "HUDs/OptionBaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/MainGameInstance.h"

void UOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンにイベントを登録 
	graphicsButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnGraphicsButtonClicked);
	gameplayButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnGameplayButtonClicked);
	audioButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnAudioButtonClicked);
	closeButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnCloseButtonClicked);
	applyButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnApplyButtonClicked);
	revertButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::OnRevertButtonClicked);
}

void UOptionMenuWidget::OnGraphicsButtonClicked()
{
	// オーバーレイ内のすべてのサブウィジェットをクリアしてから新しいものを追加
	RemoveSubWidgets();
	CreateSubWidget(graphicOptionWidgetClass, graphicOptionWidget, subWBPOverlay);
}

void UOptionMenuWidget::OnGameplayButtonClicked()
{
	RemoveSubWidgets();
	CreateSubWidget(gameplayOptionWidgetClass, gameplayOptionWidget, subWBPOverlay);
}

void UOptionMenuWidget::OnAudioButtonClicked()
{
	RemoveSubWidgets();
	CreateSubWidget(audioOptionWidgetClass, audioOptionWidget, subWBPOverlay);
}

void UOptionMenuWidget::OnCloseButtonClicked()
{
	// このウィジェットを外す
	RemoveFromParent();
	UUIUtilities::SetFocusUIAndInGame(this);
}

void UOptionMenuWidget::OnApplyButtonClicked()
{
	// サブウィジェットの設定を適用
	ApplySubWidgetSetting(graphicOptionWidget);
	ApplySubWidgetSetting(gameplayOptionWidget);
	ApplySubWidgetSetting(audioOptionWidget);
}

void UOptionMenuWidget::OnRevertButtonClicked()
{
	// サブウィジェットの設定を元に戻す
	RevertSubWidgetSetting(graphicOptionWidget);
	RevertSubWidgetSetting(gameplayOptionWidget);
	RevertSubWidgetSetting(audioOptionWidget);
}

void UOptionMenuWidget::CreateSubWidget(TSubclassOf<UUserWidget> widgetClass, UUserWidget*& widgetPointer, UOverlay* parentOverlay)
{
	if(widgetClass)
	{
		widgetPointer = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if(widgetPointer)
		{
			UOverlaySlot* overlaySlot = parentOverlay->AddChildToOverlay(widgetPointer);
	        if(overlaySlot)
	        {
		        overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	        }
		}
	}
}

void UOptionMenuWidget::RemoveSubWidgets()
{
	subWBPOverlay->ClearChildren();
	// 全部のサブウィジェットを削除したのでポインタをnullptrにする
	graphicOptionWidget = nullptr;
	gameplayOptionWidget = nullptr;
	audioOptionWidget = nullptr;
}

void UOptionMenuWidget::ApplySubWidgetSetting(UUserWidget* widget)
{
	if(widget)
	{
		auto optionBaseWidget = Cast<UOptionBaseWidget>(widget);
		if(optionBaseWidget)
		{
			optionBaseWidget->ApplySetting();
		}
	}
}

void UOptionMenuWidget::RevertSubWidgetSetting(UUserWidget* widget)
{
	if(widget)
	{
		auto optionBaseWidget = Cast<UOptionBaseWidget>(widget);
		if(optionBaseWidget)
		{
			optionBaseWidget->RevertSetting();
		}
	}
}