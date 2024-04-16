// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/MenuBaseWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"
#include "Components/Button.h"
#include "Interfaces/HasSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/MainSaveGame.h"

void UMenuBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// セーブデータをロード
	hasSaveGame = Cast<IHasSaveGame>(GetGameInstance());
	
	// ボタンにイベントを登録
	optionButton->OnClicked.AddDynamic(this, &UMenuBaseWidget::OnOptionButtonClicked);
	quitButton->OnClicked.AddDynamic(this, &UMenuBaseWidget::OnQuitButtonClicked);
}

void UMenuBaseWidget::OnOptionButtonClicked()
{
	if(optionMenuWidgetClass)
	{
		auto optionMenuWidget = CreateWidget<UUserWidget>(GetWorld(), optionMenuWidgetClass);
		if(optionMenuWidget)
		{
			optionMenuWidget->AddToViewport();
			UUIUtilities::SetFocusUI(this);
		}
	}
}

void UMenuBaseWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
