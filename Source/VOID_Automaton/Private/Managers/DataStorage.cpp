// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DataStorage.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

UDataStorage::UDataStorage()
{
}

UDataTable* UDataStorage::LoadSyncDataTable(EDataTableType dataTableType)
{
	// マップにデータテーブルが存在するか確認
	if(dataTableMap.Contains(dataTableType))
	{
		// データテーブルの種類を取得
		TSoftObjectPtr<UDataTable> dataTableToLoad = dataTableMap[dataTableType];
		
		UDataTable* loadedDataTable = dataTableToLoad.LoadSynchronous();
		// 同期ロード
		if(loadedDataTable)
		{
			// ロードしたデータを格納
			SaveLoadedDataTable(loadedDataTable, dataTableType);
			// ロードしたデータテーブルを返す
			return loadedDataTable;
		}
	}

	// dataTableMapにデータテーブルが存在しない場合、またはロードに失敗した場合はnullptrを返す
	return nullptr;
}

void UDataStorage::LoadAsyncDataTable(EDataTableType dataTableType, TFunction<void(UDataTable*)> OnLoadComplete)
{
	// マップにデータテーブルが存在するか確認
	if(dataTableMap.Contains(dataTableType))
	{
		// データテーブルの種類を取得
		TSoftObjectPtr<UDataTable> dataTableToLoad = dataTableMap[dataTableType];
		if(dataTableToLoad.ToSoftObjectPath().IsValid())
		{
			// データテーブルを非同期ロード
			FStreamableManager& streamable = UAssetManager::GetStreamableManager();
			streamable.RequestAsyncLoad(
				dataTableToLoad.ToSoftObjectPath(),
				// ロード完了時の処理
				FStreamableDelegate::CreateLambda([this, OnLoadComplete, dataTableToLoad, dataTableType]()
								{
									// ロードしたデータを格納（使わなくなったら削除）
									SaveLoadedDataTable(dataTableToLoad.Get(), dataTableType);
									// ロード完了時の処理を実行
									OnLoadComplete(dataTableToLoad.Get());
								}));
		}
	}
}

void UDataStorage::UnloadDataTable(EDataTableType dataTableType)
{
	if(dataTableMap.Contains(dataTableType))
	{
		TSoftObjectPtr<UDataTable> dataTableToUnload = dataTableMap[dataTableType];
		if(dataTableToUnload.IsValid())
		{
			// ロードされたポインターをリセット
			dataTableToUnload.Reset();
			// ロード済みデータテーブルを削除
			if(dataTableType == EDataTableType::BuffList)
			{
				//UE_LOG(LogTemp, Warning, TEXT("BuffListDataTable unloaded."));
				loadedBuffListDataTable = nullptr;
			}
			else if(dataTableType == EDataTableType::QuestList)
			{
				//UE_LOG(LogTemp, Warning, TEXT("QuestListDataTable unloaded."));
				loadedQuestListDataTable = nullptr;
			}
			else if(dataTableType == EDataTableType::ItemList)
			{
				//UE_LOG(LogTemp, Warning, TEXT("ItemListDataTable unloaded."));
				loadedItemListDataTable = nullptr;
			}
		}
	}
}

void UDataStorage::SaveLoadedDataTable(UDataTable* loadedDataTable, EDataTableType dataTableType)
{
	if (loadedDataTable == nullptr) // 現在デバッグ用
	{
		return;
	}

	FString loadedDataTableName = loadedDataTable->GetName();
	if(loadedDataTableName.IsEmpty())
	{
		return;
	}

	switch(dataTableType)
	{
	case EDataTableType::BuffList:
		loadedBuffListDataTable = loadedDataTable;
		break;
	case EDataTableType::QuestList:
		loadedQuestListDataTable = loadedDataTable;
		break;
	case EDataTableType::ItemList:
		loadedItemListDataTable = loadedDataTable;
		break;
	default:
		break;
	}
}

UDataTable* UDataStorage::GetBuffListDataTable()
{
	return loadedBuffListDataTable;
}

UDataTable* UDataStorage::GetQuestListDataTable()
{
	return loadedQuestListDataTable;
}

UDataTable* UDataStorage::GetItemListDataTable()
{
	return loadedItemListDataTable;
}
