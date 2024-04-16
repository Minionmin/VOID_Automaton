// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/LevelSelectorWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Interfaces/HasSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ListDataEntry/ItemDropListDataEntry.h"
#include "ListDataEntry/QuestListDataEntry.h"
#include "Managers/DataStorage.h"
#include "Structures/ItemListStruct.h"

void ULevelSelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// スタートボタンが押されたら任意のレベルを開く
	startButton->OnClicked.AddDynamic(this, &ULevelSelectorWidget::OnStartButtonClicked);
	closeButton->OnClicked.AddDynamic(this, &ULevelSelectorWidget::OnCloseButtonClicked);

	InitializeQuestList();
}

void ULevelSelectorWidget::InitializeQuestList()
{
	// GameInstanceからデータテーブルを取得する
	UDataTable* questListDataTable = nullptr;
	if(const auto dataStorage = UMasterUtilities::GetDataStorage(this))
	{
		questListDataTable = dataStorage->LoadSyncDataTable(EDataTableType::QuestList);
		itemDataTable = dataStorage->LoadSyncDataTable(EDataTableType::ItemList);
	}

	if(questListDataTable)
	{
		for(const auto data : questListDataTable->GetRowMap())
		{
			// 各データ行からクエストリスト構造体を取得する
			// *重要* data.ValueがFQuestListStruct型であることを確認してからキャストする
			FQuestListStruct* questData = reinterpret_cast<FQuestListStruct*>(data.Value);
			if(questData)
			{
				// クエストリストのエントリーを作成し、データを初期化してからリストビューに追加する
				UQuestListDataEntry* questDataEntry = NewObject<UQuestListDataEntry>(this);
				questDataEntry->InitializeData(questData->questIcon, questData->questName, questData->questImage, questData->levelName, questData->dropItemIDList, this);
				questListView->AddItem(questDataEntry);
			}
		}
	}
}

void ULevelSelectorWidget::OnStartButtonClicked()
{
	if(levelName != "None")
	{
		// プレイヤーのコントローラーの入力モードをゲームモードに戻す
		UUIUtilities::SetFocusInGame(this);

		// レベル間にデータを渡すようにGameInstanceに伝える
		const auto hasSaveGame = Cast<IHasSaveGame>(GetGameInstance());
		hasSaveGame->SetDataNextLevel_Implementation(true);
		hasSaveGame->SetCombatModeNextLevel_Implementation(true);

		// QuestManagerに渡す情報を設定する
		if(const auto questManager = UMasterUtilities::GetQuestManager(this))
		{
			questManager->SetQuestName(questName);
			questManager->SetItemDropIDList(itemDropIDList);
			questManager->ClearQuestStat();
		}

		// Widgetが閉じる時の処理を実行
		OnCloseButtonClicked();
		
		UGameplayStatics::OpenLevel(GetWorld(), levelName);
	}
}

void ULevelSelectorWidget::OnCloseButtonClicked()
{
	// Widgetをプレイヤーの画面から外す
	RemoveFromParent();

	// もう使わなくなったデータを削除
	if(const auto dataStorage = UMasterUtilities::GetDataStorage(this))
	{
		dataStorage->UnloadDataTable(EDataTableType::QuestList);
		dataStorage->UnloadDataTable(EDataTableType::ItemList);
	}
	
	// プレイヤーのコントローラーの入力モードをゲームモードに戻す
	UUIUtilities::SetFocusInGame(this);
}

void ULevelSelectorWidget::SetLevelName(FName newLevelName)
{
	levelName = newLevelName;
}

void ULevelSelectorWidget::SetQuestName(FText newQuestName)
{
	questName = newQuestName;
}

void ULevelSelectorWidget::SetQuestImage(UTexture2D* newQuestImage)
{
	questImage->SetBrushFromTexture(newQuestImage);
}

void ULevelSelectorWidget::SetItemDropList(const TArray<int>& itemDropIDArray)
{
	// 前に選択されたクエストのドロップアイテムリストビューをクリアする
	itemDropListView->ClearListItems();

	// ドロップアイテムIDリストを更新する
	itemDropIDList.Empty();
	for(auto id : itemDropIDArray)
	{
		itemDropIDList.Add(id);
	}
	
	if(itemDataTable)
	{
		// アイテムIDリストからアイテムデータを取得し、リストビューに追加する
		for(auto itemID : itemDropIDArray)
		{
			const FName rowName = FName(*FString::FromInt(itemID));
			FItemListStruct* itemData = itemDataTable->FindRow<FItemListStruct>(rowName, TEXT("Item ID not found"));
			if(itemData && itemData->itemID == itemID)
			{
				UItemDropListDataEntry* itemDropDataEntry = NewObject<UItemDropListDataEntry>(this);
				itemDropDataEntry->InitializeData(itemData->itemID, itemData->itemIcon, itemData->itemName,
					itemData->itemRarity, itemData->itemDescription, itemData->dropRate, 0);
				itemDropListView->AddItem(itemDropDataEntry);
			}
		}
	}
}
