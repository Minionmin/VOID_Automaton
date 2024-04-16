// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/OptionBaseWidget.h"

#include "Interfaces/HasSaveGame.h"

void UOptionBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	hasSaveGame = Cast<IHasSaveGame>(GetGameInstance());
	if(hasSaveGame)
	{
		bIsNewGame = hasSaveGame->GetNewGameStarted_Implementation();
	}
}

void UOptionBaseWidget::ApplySetting()
{
	if(hasSaveGame)
	{
		hasSaveGame->SetNewGameStarted_Implementation(false);
		bIsNewGame = false;
	}
}
