// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/InGameMenuWidget.h"

#include "VOIDCharacter.h"
#include "Components/Button.h"
#include "Interfaces/HasSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/MainGameInstance.h"
#include "Saves/MainSaveGame.h"

void UInGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// コンバット中はセーブできない
	if(hasSaveGame->GetCombatModeNextLevel_Implementation())
	{
		saveButton->SetIsEnabled(false);
		loadButton->SetIsEnabled(false);
		return;
	}

	saveButton->SetIsEnabled(true);
	loadButton->SetIsEnabled(true);
	
	// プレイヤーキャラクターを取得
	VOIDCharacter = Cast<AVOIDCharacter>(UGameplayStatics::GetPlayerCharacter(this , 0));
	
	// ボタンにイベントを登録
	saveButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnSaveButtonClicked);
	loadButton->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnLoadButtonClicked);
}

void UInGameMenuWidget::OnSaveButtonClicked()
{
	if(hasSaveGame && VOIDCharacter)
	{
		hasSaveGame->SavePlayerStruct_Implementation(VOIDCharacter->GetPlayerSaveData(), false);
		hasSaveGame->SetNewGameStarted_Implementation(false);
	}
}

void UInGameMenuWidget::OnLoadButtonClicked()
{
	LoadPlayerSaveData();
}

void UInGameMenuWidget::LoadPlayerSaveData()
{
	if(const auto gameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		// 前回のバフリストデータをGameInstanceにロードしておく
		gameInstance->LoadBuffListFromSaveFile();
	}
	
	if(hasSaveGame && VOIDCharacter)
	{
		if(const auto saveData = hasSaveGame->GetSaveGame_Implementation())
		{
			VOIDCharacter->UsePlayerSaveData(saveData->GetPlayerSaveData());
		}
	}
}
