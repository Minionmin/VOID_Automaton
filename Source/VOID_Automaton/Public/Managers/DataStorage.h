// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStorage.generated.h"

// バフデータテーブル
// ** ゲームを初めて起動する時に、セーブファイルからプレイヤーが前回持っているバフを比較するため
// ** バフショップにバフ一覧を表示するため
// ** プレイヤー

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	DTT_None,
	BuffList,
	QuestList,
	ItemList
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOID_AUTOMATON_API UDataStorage : public UObject
{
	GENERATED_BODY()

public:

	UDataStorage();

	// Loader関数
	UDataTable* LoadSyncDataTable(EDataTableType dataTableType);
	void LoadAsyncDataTable(EDataTableType dataTableType, TFunction<void(UDataTable*)> OnLoadComplete);
	void UnloadDataTable(EDataTableType dataTableType);
	
	// ロードコールバック
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SaveLoadedDataTable(UDataTable* loadedDataTable, EDataTableType dataTableType);

	// データゲッター
	UFUNCTION(BlueprintCallable, Category = "Data")
	UDataTable* GetBuffListDataTable();
	UFUNCTION(BlueprintCallable, Category = "Data")
	UDataTable* GetQuestListDataTable();
	UFUNCTION(BlueprintCallable, Category = "Data")
	UDataTable* GetItemListDataTable();
	
private:

	// データテーブルソフト参照
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TMap<EDataTableType, TSoftObjectPtr<UDataTable>> dataTableMap;
	
	// ロード済みデータテーブル
	UPROPERTY()
	UDataTable* loadedBuffListDataTable = nullptr;
	UPROPERTY()
	UDataTable* loadedQuestListDataTable = nullptr;
	UPROPERTY()
	UDataTable* loadedItemListDataTable = nullptr;
};
