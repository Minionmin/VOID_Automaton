// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GameOverWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/Button.h"
#include "Interfaces/HasSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ボタンのイベントバインディング
	revengeButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnRevengeButtonClicked);
	returnButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnReturnButtonClicked);
}

void UGameOverWidget::OnRevengeButtonClicked()
{
	// 現在のレベルを再開する
	UUIUtilities::SetFocusInGame(this);
	UGameplayStatics::OpenLevel(this, GetWorld()->GetFName());
}

void UGameOverWidget::OnReturnButtonClicked()
{
	// ベースに戻る
	if(returnLevelName != "None")
	{
		if(auto hasSaveGame = Cast<IHasSaveGame>(GetGameInstance()))
		{
			hasSaveGame->SetCombatModeNextLevel_Implementation(false);
		}
		
		UUIUtilities::SetFocusInGame(this);
		UGameplayStatics::OpenLevel(GetWorld(), returnLevelName);
	}
}
