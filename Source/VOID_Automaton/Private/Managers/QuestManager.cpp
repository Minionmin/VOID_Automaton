// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/QuestManager.h"

#include "MyCustomUnrealLibrary.h"
#include "HUDs/QuestResultWidget.h"
#include "Managers/DataStorage.h"
#include "Components\PlayerInventoryComponent.h"

UQuestManager::UQuestManager()
{
}

FText UQuestManager::GetQuestName() const
{
	return questName;
}

TArray<int> UQuestManager::GetItemDropIDList() const
{
	return itemDropIDList;
}

void UQuestManager::SetPlayerInventory(UPlayerInventoryComponent* targetPlayerInventory)
{
	if(playerInventory)
	{
		// Log item amount in player inventory
		for(const auto itemID : playerInventory->GetItemIDList())
		{
			const int itemAmount = playerInventory->GetItemAmount(itemID);
			UE_LOG(LogTemp, Warning, TEXT("ItemID: %d, Amount: %d"), itemID, itemAmount);
		}
	}
	if(!playerInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetPlayerInventory"));
		playerInventory = targetPlayerInventory;
	}
}

#pragma region levelSelectorUI
// クエスト選択UIで「出発」が押される時にデータが渡される
void UQuestManager::SetQuestName(FText name)
{
	questName = name;
}

void UQuestManager::SetItemDropIDList(const TArray<int>& itemDropIDArray)
{
	ClearItemDropIDList();
	
	// Deep copy（プレイヤーが勝利した時にドロップアイテムを取得するため）
	for(const auto itemID : itemDropIDArray)
	{
		itemDropIDList.Add(itemID);
	}
}

void UQuestManager::ClearQuestStat()
{
	questStat = FQuestStatStruct();
}
#pragma endregion levelSelectorUI

void UQuestManager::ClearItemDropIDList()
{
	itemDropIDList.Empty();
}

void UQuestManager::StartQuestTimer()
{
	startTime = FDateTime::UtcNow();
}

void UQuestManager::StopQuestTimer()
{
	stopTime = FDateTime::UtcNow();
}

FTimespan UQuestManager::CalculateTimeTaken() const
{
	return stopTime - startTime;
}

void UQuestManager::AddHeadshotCount()
{
	questStat.headshotCount++;
}

void UQuestManager::AddBodyshotCount()
{
	questStat.bodyshotCount++;
}

void UQuestManager::AddDamageDealtTotal(int additionalDamage)
{
	questStat.damageDealtTotal += additionalDamage;
}

void UQuestManager::AddDamageTakenTotal(int additionalDamage)
{
	questStat.damageTakenTotal += additionalDamage;
}

void UQuestManager::AddDashCount()
{
	questStat.dashCount++;
}

void UQuestManager::AddShotCount()
{
	questStat.totalShotCount++;
}

void UQuestManager::ShowQuestResultWidget()
{
	// イベントの登録解除
	OnQuestCompleted.Broadcast();
	OnQuestCompleted.Clear();

	// クエストタイマーを停止
	StopQuestTimer();
	
	if(questResultWidgetClass)
	{
		questResultWidget = CreateWidget<UQuestResultWidget>(GetWorld(), questResultWidgetClass);
		if(questResultWidget)
		{
			questResultWidget->AddToViewport();
		}

		// クエスト結果ウィジェットに情報を渡す
		questResultWidget->SetQuestName(questName);
		questResultWidget->SetQuestTimer(FText::FromString(CalculateTimeTaken().ToString(TEXT("%m:%s:%f"))));
		questResultWidget->SetHeadshotCount(questStat.headshotCount);
		questResultWidget->SetBodyshotCount(questStat.bodyshotCount);
		questResultWidget->SetDamageDealt(questStat.damageDealtTotal);
		questResultWidget->SetDamageTaken(questStat.damageTakenTotal);
		questResultWidget->SetDashAmount(questStat.dashCount);
		questResultWidget->SetAccuracyText(CalculateAccuracy());
		questResultWidget->SetItemDropList(CalculateItemDropList());

		UUIUtilities::SetFocusUI(this);
	}
}

void UQuestManager::ShowGameOverWidget()
{
	// イベントの登録解除
	OnQuestCompleted.Clear();
	
	// クエストタイマーを停止
	StopQuestTimer();
	
	if(gameOverWidgetClass)
	{
		gameOverWidget = CreateWidget<UUserWidget>(GetWorld(), gameOverWidgetClass);
		if(gameOverWidget)
		{
			gameOverWidget->AddToViewport();
		}
		UUIUtilities::SetFocusUI(this);
	}
}

float UQuestManager::CalculateAccuracy()
{
	return static_cast<float>(questStat.headshotCount + questStat.bodyshotCount) /  static_cast<float>(questStat.totalShotCount);
}

TArray<TPair<FItemListStruct*, int>> UQuestManager::CalculateItemDropList()
{
	// ドロップアイテムリストを作成
	TArray<TPair<FItemListStruct*, int>> itemPairArray;
	
	// アイテムリストデータを取得
	const UDataTable* itemListDataTable = nullptr;
	const auto dataStorage = UMasterUtilities::GetDataStorage(this);
	if(dataStorage)
	{
		itemListDataTable = dataStorage->LoadSyncDataTable(EDataTableType::ItemList);
	}
	if(!itemListDataTable) return itemPairArray;
	
	if(itemDropIDList.Num() > 0)
	{
		// ドロップアイテムリストを作成
		for(const auto itemID : itemDropIDList)
		{
			// ドロップアイテムのデータを取得
			const FName rowName = FName(*FString::FromInt(itemID));
			FItemListStruct* itemData = itemListDataTable->FindRow<FItemListStruct>(rowName, TEXT("Item ID not found"));
			if(itemData && itemData->itemID == itemID)
			{
				// ドロップ率に応じてアイテムをドロップ
				const int random = FMath::RandRange(1, 100);
				if(random <= itemData->dropRate)
				{
					// ドロップアイテムの個数を設定 + ドロップしたアイテムのみリストに追加
					int itemAmount = FMath::RandRange(1, itemData->maximumDropAmount);
					TPair<FItemListStruct*, int> itemPair = TPair<FItemListStruct*, int>(itemData, itemAmount);
					itemPairArray.Add(itemPair);

					// ドロップアイテムのデータをPlayerInventoryに追加
					if(playerInventory)
					{
						playerInventory->AddItem(itemID, itemAmount);
					}
				}
			}
		}
	}

	// アンロードアイテムテーブル
	dataStorage->UnloadDataTable(EDataTableType::ItemList);

	return itemPairArray;
}
