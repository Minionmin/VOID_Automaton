// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/AudioOptionWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/Slider.h"
#include "Interfaces/HasSaveGame.h"
#include "Managers/AudioManager.h"
#include "Saves/MainGameInstance.h"
#include "Saves/MainSaveGame.h"

class IHasSaveGame;

void UAudioOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gameInstance = Cast<UMainGameInstance>(GetGameInstance());

	// スライダーの値が変更されたときのイベントをバインド
	masterSlider->OnValueChanged.AddDynamic(this, &UAudioOptionWidget::OnMasterValueChanged);
	bgmSlider->OnValueChanged.AddDynamic(this, &UAudioOptionWidget::OnBGMValueChanged);
	sfxSlider->OnValueChanged.AddDynamic(this, &UAudioOptionWidget::OnSFXValueChanged);

	InitializeSetting();
}

void UAudioOptionWidget::ApplySetting()
{
	// NewGameフラグを更新
	Super::ApplySetting();
	
	// プレイヤーのセーブデータを更新
	if(hasSaveGame)
	{
		// セーブデータを取得
		auto saveData = hasSaveGame->GetSaveGame_Implementation()->GetPlayerSaveData();

		// セーブデータを一部変更（音量をセット）
		saveData.masterVolume = masterSlider->GetValue();
		saveData.bgmVolume = bgmSlider->GetValue();
		saveData.sfxVolume = sfxSlider->GetValue();

		// セーブデータを保存
		hasSaveGame->SavePlayerStruct_Implementation(saveData, false);
	}
}

void UAudioOptionWidget::RevertSetting()
{
	masterSlider->SetValue(1.0f);
	bgmSlider->SetValue(1.0f);
	sfxSlider->SetValue(1.0f);
}

void UAudioOptionWidget::InitializeSetting()
{
	// セーブデータがあるかつNewGameではない場合、セーブデータから音量を取得
	if(hasSaveGame && !hasSaveGame->GetNewGameStarted_Implementation())
	{
		auto saveData = hasSaveGame->GetSaveGame_Implementation()->GetPlayerSaveData();
		masterSlider->SetValue(saveData.masterVolume);
		bgmSlider->SetValue(saveData.bgmVolume);
		sfxSlider->SetValue(saveData.sfxVolume);
	}
	else
	{
		// セーブデータがないかNewGameの場合、デフォルト値をセット
		masterSlider->SetValue(1.0f);
		bgmSlider->SetValue(1.0f);
		sfxSlider->SetValue(1.0f);
	}
}

void UAudioOptionWidget::OnMasterValueChanged(float value)
{
	if(!gameInstance) return;
	
	// 全体の音量を変更
	if(IManageSingleton* singletonManager = Cast<IManageSingleton>(gameInstance))
	{
		singletonManager->GetAudioManager_Implementation()->SetMasterVolume(value);	
	}
	
	// テキストを更新
	UUIUtilities::UpdateTextBlock(masterText, value);
}

void UAudioOptionWidget::OnBGMValueChanged(float value)
{
	if(!gameInstance) return;
	
	// BGMの音量を変更
	if(IManageSingleton* singletonManager = Cast<IManageSingleton>(gameInstance))
	{
		singletonManager->GetAudioManager_Implementation()->SetBGMVolume(value);
	}
	
	// テキストを更新
	UUIUtilities::UpdateTextBlock(bgmText, value);
}

void UAudioOptionWidget::OnSFXValueChanged(float value)
{
	if(!gameInstance) return;
	
	// SFXの音量を変更
	if(IManageSingleton* singletonManager = Cast<IManageSingleton>(gameInstance))
	{
		singletonManager->GetAudioManager_Implementation()->SetSFXVolume(value);
	}
	
	// テキストを更新
	UUIUtilities::UpdateTextBlock(sfxText, value);
}
