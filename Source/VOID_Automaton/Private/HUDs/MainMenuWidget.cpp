// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/MainMenuWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/Button.h"
#include "Interfaces/HasSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/MainGameInstance.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンにイベントを登録
	startButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartButtonClicked);
	continueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinueButtonClicked);
}

void UMainMenuWidget::OpenBaseLevel()
{
	if(startLevelName != "None")
	{
		UGameplayStatics::OpenLevel(GetWorld(), startLevelName);
		UUIUtilities::SetFocusInGame(this);
	}
}

void UMainMenuWidget::OnStartButtonClicked()
{
	if(hasSaveGame)
	{
		hasSaveGame->SetNewGameStarted_Implementation(true);	
	}
	OpenBaseLevel();
}

void UMainMenuWidget::OnContinueButtonClicked()
{
	// メインゲームインスタンスにメインメニューからの遷移であることを伝える
	// レベルを遷移する際にセーブデータをロードするかどうかを判断するため
	if(auto gameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		// 前回のセーブデータをロードする
		// バフ
		gameInstance->LoadBuffListFromSaveFile();
		
		if(hasSaveGame)
		{
			// プレイヤーにセーブファイルのデータに基づいてゲームを再開するように指示する
			hasSaveGame->SetMainMenuContinue_Implementation(true);
		}	
	}
	
	OpenBaseLevel();
}
