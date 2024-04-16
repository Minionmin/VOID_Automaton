// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/QuestResultWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Interfaces/HasSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ListDataEntry/ItemDropListDataEntry.h"

#define BASE_SHIP_LEVEL_NAME "BaseShip"

void UQuestResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	backToShipButton->OnClicked.AddDynamic(this, &UQuestResultWidget::OnBackToShipButtonClicked);
}

void UQuestResultWidget::SetQuestName(FText name)
{
	 questNameText->SetText(name);
}

void UQuestResultWidget::SetQuestTimer(FText time)
{
	questTimerText->SetText(time);
}

void UQuestResultWidget::SetHeadshotCount(int count)
{
	UUIUtilities::UpdateTextBlock(headshotText ,count);
}

void UQuestResultWidget::SetBodyshotCount(int count)
{
	UUIUtilities::UpdateTextBlock(bodyshotText ,count);
}

void UQuestResultWidget::SetDamageDealt(int damage)
{
	UUIUtilities::UpdateTextBlock(damageDealtText ,damage);
}

void UQuestResultWidget::SetDamageTaken(int damage)
{
	UUIUtilities::UpdateTextBlock(damageTakenText ,damage);
}

void UQuestResultWidget::SetDashAmount(int amount)
{
	UUIUtilities::UpdateTextBlock(dashAmountText ,amount);
}

void UQuestResultWidget::SetAccuracyText(float overallAccuracy)
{
	FNumberFormattingOptions numberFormat;
	numberFormat.MaximumFractionalDigits = 2;
	accuracyText->SetText(FText::AsPercent(overallAccuracy, &numberFormat));
}

void UQuestResultWidget::SetItemDropList(const TArray<TPair<FItemListStruct*, int>>& itemPairArray)
{
	for(const auto itemPair : itemPairArray)
	{
		UItemDropListDataEntry* itemDropDataEntry = NewObject<UItemDropListDataEntry>(this);
		itemDropDataEntry->InitializeData(itemPair.Key->itemID, itemPair.Key->itemIcon,
			itemPair.Key->itemName, itemPair.Key->itemRarity, itemPair.Key->itemDescription, itemPair.Key->dropRate, itemPair.Value);
		itemDropListView->AddItem(itemDropDataEntry);
	}
}

void UQuestResultWidget::OnBackToShipButtonClicked()
{
	if(auto hasSaveGame = Cast<IHasSaveGame>(GetGameInstance()))
	{
		hasSaveGame->SetCombatModeNextLevel_Implementation(false);
	}
	
	UUIUtilities::SetFocusInGame(this);
	UGameplayStatics::OpenLevel(this, BASE_SHIP_LEVEL_NAME);
}
