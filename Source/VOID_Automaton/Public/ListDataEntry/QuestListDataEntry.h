// Fill out your copyright notice in the Description page of Project Settings.

/*
 * クエストリストのデータ格納オブジェクトとして使用するクラス
 * メインWidget（LevelSelectorWidget）がこのクラスのインスタンスを作成し、
 * データを初期化してリストビューに追加する
 */

#pragma once

#include "CoreMinimal.h"
#include "Structures/QuestListStruct.h"
#include "QuestListDataEntry.generated.h"

class ULevelSelectorWidget;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOID_AUTOMATON_API UQuestListDataEntry : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void InitializeData(UTexture2D* questIcon, FText questName, UTexture2D* questImage, FName levelName, const TArray<int>& dropItemIDList, ULevelSelectorWidget* levelSelectorWidget);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuestListStruct questData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelSelectorWidget* levelSelectorWidget;
	
};
