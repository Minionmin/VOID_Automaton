// Fill out your copyright notice in the Description page of Project Settings.

/*
 * 任意のクエストのアイテムドロップデータ格納オブジェクトとして使用するクラス
 * メインWidget（LevelSelectorWidget）でクエストを選択すると、このクラスのインスタンスが作成され、
 * データを初期化してリストビューに追加する
 */

#pragma once

#include "CoreMinimal.h"
#include "Structures/ItemListStruct.h"
#include "ItemDropListDataEntry.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UItemDropListDataEntry : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitializeData(int itemID, UTexture2D* itemIcon, FText itemName, EItemRarity itemRarity, FText itemDesc, float itemDropRate, int itemAmount);
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FItemListStruct itemData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int itemAmount = 0;
};
