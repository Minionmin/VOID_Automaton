// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GameplayOptionWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"
#include "Components/PlayerInputManagerComponent.h"
#include "Components/Slider.h"
#include "Interfaces/HasSaveGame.h"
#include "Saves/MainSaveGame.h"

// スライダーのデフォルト最小値と最大値
#define SLIDER_MIN_VALUE 0.0f
#define SLIDER_MAX_VALUE 1.0f

void UGameplayOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// PlayerInputManagerComponentを取得
	AVOIDCharacter* VOIDCharacter = Cast<AVOIDCharacter>(GetOwningPlayerPawn());
	if(VOIDCharacter)
	{
		inputManagerComponent = VOIDCharacter->GetInputManagerComponent();
	}

	// OnValueChangedイベントに関数をバインド（プレイヤー設定とテキストの更新）
	mouseHipFireSlider->OnValueChanged.AddDynamic(this, &UGameplayOptionWidget::OnMouseHipFireValueChanged);
	mouseAimSlider->OnValueChanged.AddDynamic(this, &UGameplayOptionWidget::OnMouseAimValueChanged);
	controllerHipFireSlider->OnValueChanged.AddDynamic(this, &UGameplayOptionWidget::OnControllerHipFireValueChanged);
	controllerAimSlider->OnValueChanged.AddDynamic(this, &UGameplayOptionWidget::OnControllerAimValueChanged);
	
	// プレイヤーのセーブデータから設定を読み込む
	InitializeSetting();
}

void UGameplayOptionWidget::ApplySetting()
{
	// NewGameフラグを更新
	Super::ApplySetting();
	
	// プレイヤーのセーブデータを更新
	if(hasSaveGame && inputManagerComponent)
	{
		auto saveData = hasSaveGame->GetSaveGame_Implementation()->GetPlayerSaveData();
		
		saveData.mouseHipTurnRate = UMathUtilities::ScaleValueInOtherRange(
			mouseHipFireSlider->GetValue(),
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE,
			UPlayerInputManagerComponent::MOUSE_HIP_TURN_RATE_MIN,
			UPlayerInputManagerComponent::MOUSE_HIP_TURN_RATE_MAX
		);
		
		saveData.mouseAimTurnRate = UMathUtilities::ScaleValueInOtherRange(
			mouseAimSlider->GetValue(),
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE,
			UPlayerInputManagerComponent::MOUSE_AIM_TURN_RATE_MIN,
			UPlayerInputManagerComponent::MOUSE_AIM_TURN_RATE_MAX
		);
		
		saveData.controllerHipTurnRate = UMathUtilities::ScaleValueInOtherRange(
			controllerHipFireSlider->GetValue(),
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE,
			UPlayerInputManagerComponent::CONTROLLER_HIP_TURN_RATE_MIN,
			UPlayerInputManagerComponent::CONTROLLER_HIP_TURN_RATE_MAX
		);
		
		saveData.controllerAimTurnRate = UMathUtilities::ScaleValueInOtherRange(
			controllerAimSlider->GetValue(),
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE,
			UPlayerInputManagerComponent::CONTROLLER_AIM_TURN_RATE_MIN,
			UPlayerInputManagerComponent::CONTROLLER_AIM_TURN_RATE_MAX
		);
		
		hasSaveGame->SavePlayerStruct_Implementation(saveData, false);
	}
}

void UGameplayOptionWidget::RevertSetting()
{
	// 全てのスライダーの値をリセット（1.0fにする）
	mouseHipFireSlider->SetValue(1.0f);
	mouseAimSlider->SetValue(1.0f);
	controllerHipFireSlider->SetValue(1.0f);
	controllerAimSlider->SetValue(1.0f);
}

void UGameplayOptionWidget::InitializeSetting()
{
	// セーブデータが存在し、新規ゲームが開始されていない場合データを読み込む
	if(hasSaveGame && !hasSaveGame->GetNewGameStarted_Implementation())
	{
		auto saveData = hasSaveGame->GetSaveGame_Implementation()->GetPlayerSaveData();
		// スライダーの初期化
		mouseHipFireSlider->SetValue(UMathUtilities::ScaleValueInOtherRange(
			saveData.mouseHipTurnRate,
			UPlayerInputManagerComponent::MOUSE_HIP_TURN_RATE_MIN,
			UPlayerInputManagerComponent::MOUSE_HIP_TURN_RATE_MAX,
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE
			));
		mouseAimSlider->SetValue(UMathUtilities::ScaleValueInOtherRange(
			saveData.mouseAimTurnRate,
			UPlayerInputManagerComponent::MOUSE_AIM_TURN_RATE_MIN,
			UPlayerInputManagerComponent::MOUSE_AIM_TURN_RATE_MAX,
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE
			));
		controllerHipFireSlider->SetValue(UMathUtilities::ScaleValueInOtherRange(
			saveData.controllerHipTurnRate,
			UPlayerInputManagerComponent::CONTROLLER_HIP_TURN_RATE_MIN,
			UPlayerInputManagerComponent::CONTROLLER_HIP_TURN_RATE_MAX,
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE
			));
		controllerAimSlider->SetValue(UMathUtilities::ScaleValueInOtherRange(
			saveData.controllerAimTurnRate,
			UPlayerInputManagerComponent::CONTROLLER_AIM_TURN_RATE_MIN,
			UPlayerInputManagerComponent::CONTROLLER_AIM_TURN_RATE_MAX,
			SLIDER_MIN_VALUE,
			SLIDER_MAX_VALUE
			));
	}
	else
	{
		// デフォルト値を設定
		RevertSetting();
	}
}

void UGameplayOptionWidget::OnMouseHipFireValueChanged(float value)
{
	const float scaledValue = UMathUtilities::ScaleValueInOtherRange(
		value,
		SLIDER_MIN_VALUE,
		SLIDER_MAX_VALUE,
		UPlayerInputManagerComponent::MOUSE_HIP_TURN_RATE_MIN,
		UPlayerInputManagerComponent::MOUSE_HIP_TURN_RATE_MAX
		);

	if(inputManagerComponent)
	{
		inputManagerComponent->SetMouseHipTurnRate(scaledValue);	
	}
	UUIUtilities::UpdateTextBlock(mouseHipFireText, scaledValue);
}

void UGameplayOptionWidget::OnMouseAimValueChanged(float value)
{
	const float scaledValue = UMathUtilities::ScaleValueInOtherRange(
		value,
		SLIDER_MIN_VALUE,
		SLIDER_MAX_VALUE,
		UPlayerInputManagerComponent::MOUSE_AIM_TURN_RATE_MIN,
		UPlayerInputManagerComponent::MOUSE_AIM_TURN_RATE_MAX
		);

	if(inputManagerComponent)
	{
		inputManagerComponent->SetMouseAimingTurnRate(scaledValue);
	}
	UUIUtilities::UpdateTextBlock(mouseAimText, scaledValue);
}

void UGameplayOptionWidget::OnControllerHipFireValueChanged(float value)
{
	const float scaledValue = UMathUtilities::ScaleValueInOtherRange(
		value,
		SLIDER_MIN_VALUE, SLIDER_MAX_VALUE,
		UPlayerInputManagerComponent::CONTROLLER_HIP_TURN_RATE_MIN,
		UPlayerInputManagerComponent::CONTROLLER_HIP_TURN_RATE_MAX
		);

	if(inputManagerComponent)
	{
		inputManagerComponent->SetControllerHipTurnRate(scaledValue);	
	}
	UUIUtilities::UpdateTextBlock(controllerHipFireText, scaledValue);
}

void UGameplayOptionWidget::OnControllerAimValueChanged(float value)
{
	const float scaledValue = UMathUtilities::ScaleValueInOtherRange(
		value,
		SLIDER_MIN_VALUE,
		SLIDER_MAX_VALUE,
		UPlayerInputManagerComponent::CONTROLLER_AIM_TURN_RATE_MIN,
		UPlayerInputManagerComponent::CONTROLLER_AIM_TURN_RATE_MAX
		);

	if(inputManagerComponent)
	{
		inputManagerComponent->SetControllerAimingTurnRate(scaledValue);	
	}
	UUIUtilities::UpdateTextBlock(controllerAimText, scaledValue);
}
