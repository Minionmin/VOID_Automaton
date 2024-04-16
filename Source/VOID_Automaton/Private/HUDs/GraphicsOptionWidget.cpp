// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GraphicsOptionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "Interfaces/HasSaveGame.h"

void UGraphicsOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンにイベントを登録
	windowModeButton->OnClicked.AddDynamic(this, &UGraphicsOptionWidget::OnWindowModeButtonClicked);
	resolutionButton->OnClicked.AddDynamic(this, &UGraphicsOptionWidget::OnResolutionButtonClicked);
	graphicButton->OnClicked.AddDynamic(this, &UGraphicsOptionWidget::OnGraphicButtonClicked);
	vsyncButton->OnClicked.AddDynamic(this, &UGraphicsOptionWidget::OnVsyncButtonClicked);
	
	// プレイヤーのゲーム設定レファレンスを取得
	gameUserSettings = GEngine->GetGameUserSettings();
	
	// プレイヤーのセーブデータから設定を読み込む
	InitializeSetting();
}

void UGraphicsOptionWidget::InitializeSetting()
{
	// プレイヤーのゲーム設定が取得できたかつNewGameでない場合はセーブデータから設定を読み込む
	if(gameUserSettings && hasSaveGame && !hasSaveGame->GetNewGameStarted_Implementation())
	{
		// ウィンドウモードの初期化
		SetWindowMode(gameUserSettings->GetFullscreenMode());
		// 解像度の初期化
		SetResolution(gameUserSettings->GetScreenResolution());
		// グラフィック設定の初期化
		SetGraphics(gameUserSettings->GetOverallScalabilityLevel());
		// Vsyncの初期化
		IsVsyncActive = gameUserSettings->IsVSyncEnabled();
		vsyncText->SetText(FText::FromString(IsVsyncActive ? "ON" : "OFF"));
	}
	else
	{
		// プレイヤーのゲーム設定が取得できない場合またはNewGameの場合は初期設定を適用
		SetWindowMode(EWindowMode::Fullscreen);
		SetResolution(FIntPoint(1920, 1080));
		SetGraphics(1);
		IsVsyncActive = true;
		vsyncText->SetText(FText::FromString(IsVsyncActive ? "ON" : "OFF"));
	}
}

void UGraphicsOptionWidget::ApplySetting()
{
	// NewGameフラグを更新
	Super::ApplySetting();
	
	if(gameUserSettings)
	{
		gameUserSettings->ApplySettings(false);
	}
}

void UGraphicsOptionWidget::RevertSetting()
{
	// 設定を全部初期化する
	if(gameUserSettings)
	{
		SetWindowMode(EWindowMode::Fullscreen);
		SetResolution(FIntPoint(1920, 1080));
		SetGraphics(1);
		IsVsyncActive = true;
		vsyncText->SetText(FText::FromString(IsVsyncActive ? "ON" : "OFF"));
	}
}

void UGraphicsOptionWidget::OnWindowModeButtonClicked()
{
	// ウィンドウモードをインデックスに応じて更新
	if(gameUserSettings)
	{
		SetWindowMode(windowModeIndex);
	}
}

void UGraphicsOptionWidget::OnResolutionButtonClicked()
{
	// 解像度をインデックスに応じて更新
	if(gameUserSettings)
	{
		SetResolution(resolutionIndex);
	}
}

void UGraphicsOptionWidget::OnGraphicButtonClicked()
{
	// グラフィック設定をインデックスに応じて更新
	if(gameUserSettings)
	{
		SetGraphics(graphicIndex);
	}
}

void UGraphicsOptionWidget::OnVsyncButtonClicked()
{
	// Vsyncをトグル
	if(gameUserSettings)
	{
		IsVsyncActive = !IsVsyncActive;
		gameUserSettings->SetVSyncEnabled(IsVsyncActive);
		vsyncText->SetText(FText::FromString(IsVsyncActive ? "ON" : "OFF"));
	}
}

#pragma region SettingConfiguration
void UGraphicsOptionWidget::SetWindowMode(int windowModeType)
{
	// switch文でウィンドウモード+テキストを更新
	switch(windowModeType)
	{
	case 0:
		gameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		windowModeText->SetText(FText::FromString(L"フルスクリーン"));
		windowModeIndex = 1;
		break;
	case 1:
		gameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		windowModeText->SetText(FText::FromString(L"ボーダーレスフルスクリーン"));
		// 解像度を強制的に変更
		SetResolution(FIntPoint(1920, 1080));
		windowModeIndex = 2;
		break;
	case 2:
		gameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
		windowModeText->SetText(FText::FromString(L"ウィンドウ"));
		windowModeIndex = 0;
		break;
	}
}

void UGraphicsOptionWidget::SetWindowMode(EWindowMode::Type windowModeType)
{
	switch(windowModeType)
	{
	case EWindowMode::Fullscreen:
		gameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		windowModeText->SetText(FText::FromString(L"フルスクリーン"));
		windowModeIndex = 1;
		break;
	case EWindowMode::WindowedFullscreen:
		gameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		windowModeText->SetText(FText::FromString(L"ボーダーレスフルスクリーン"));
		// 解像度を強制的に変更
		SetResolution(FIntPoint(1920, 1080));
		windowModeIndex = 2;
		break;
	case EWindowMode::Windowed:
		gameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
		windowModeText->SetText(FText::FromString(L"ウィンドウ"));
		windowModeIndex = 0;
		break;
	}
}

void UGraphicsOptionWidget::SetResolution(int resolutionType)
{
	// ボーダーレスフルスクリーンの場合は解像度を変更しない
	if(gameUserSettings->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
	{
		resolutionType = 0;
	}
	
	// switch文で解像度+テキストを更新
	switch(resolutionType)
	{
	case 0:
		gameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		resolutionText->SetText(FText::FromString("1920x1080"));
		resolutionIndex = 1;
		break;
	case 1:
		gameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		resolutionText->SetText(FText::FromString("1280x720"));
		resolutionIndex = 0;
		break;
	}
}

void UGraphicsOptionWidget::SetResolution(FIntPoint screenResolution)
{
	// ボーダーレスフルスクリーンの場合は解像度を変更しない
	if(gameUserSettings->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
	{
		screenResolution.X = 1920;
		screenResolution.Y = 1080;
	}
	
	if(screenResolution.X == 1920 && screenResolution.Y == 1080)
	{
		gameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));
		resolutionIndex = 1;
	}
	else if(screenResolution.X == 1280 && screenResolution.Y == 720)
	{
		gameUserSettings->SetScreenResolution(FIntPoint(1280, 720));
		resolutionIndex = 0;
	}
	else
	{
		gameUserSettings->SetScreenResolution(screenResolution);
		resolutionIndex = 0;
	}

	// 解像度をテキストに反映
	const FString width = FString::FromInt(screenResolution.X);
	const FString height = FString::FromInt(screenResolution.Y);
	resolutionText->SetText(FText::FromString(width + "x" + height));
}

void UGraphicsOptionWidget::SetGraphics(int graphicType)
{
	// switch文でグラフィック設定+テキストを更新
	switch(graphicType)
	{
	case 0:
		gameUserSettings->SetOverallScalabilityLevel(0);
		graphicText->SetText(FText::FromString(L"低"));
		graphicIndex = 1;
		break;
	case 1:
		gameUserSettings->SetOverallScalabilityLevel(1);
		graphicText->SetText(FText::FromString(L"中"));
		graphicIndex = 2;
		break;
	case 2:
		gameUserSettings->SetOverallScalabilityLevel(2);
		graphicText->SetText(FText::FromString(L"高"));
		graphicIndex = 3;
		break;
	case 3:
		gameUserSettings->SetOverallScalabilityLevel(3);
		graphicText->SetText(FText::FromString(L"最高"));
		graphicIndex = 0;
		break;
	}
}
#pragma endregion SettingConfiguration