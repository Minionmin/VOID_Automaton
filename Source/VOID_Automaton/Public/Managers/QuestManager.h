// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structures/QuestStatStruct.h"
#include "Structures/ItemListStruct.h"
#include "UObject/Object.h"
#include "QuestManager.generated.h"

class UPlayerInventoryComponent;
class UQuestResultWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOID_AUTOMATON_API UQuestManager : public UObject
{
	GENERATED_BODY()

public:

	UQuestManager();

	/*
		イベント
	*/
	FOnQuestCompleted OnQuestCompleted;
	
	UFUNCTION(BlueprintCallable)
	FText GetQuestName() const;
	UFUNCTION(BlueprintCallable)
	TArray<int> GetItemDropIDList() const;

	// PlayerInventoryのアイテムを操作する
	UFUNCTION(BlueprintCallable)
	void SetPlayerInventory(UPlayerInventoryComponent* targetPlayerInventory);
		
	// クエストの一般情報
	UFUNCTION(BlueprintCallable)
	void SetQuestName(FText name);
	UFUNCTION(BlueprintCallable)
	void SetItemDropIDList(const TArray<int>& itemDropIDArray);
	UFUNCTION(BlueprintCallable)
	void ClearQuestStat();
	UFUNCTION(BlueprintCallable)
	void ClearItemDropIDList();

	// クエストタイマー
	UFUNCTION(BlueprintCallable)
	void StartQuestTimer();
	UFUNCTION(BlueprintCallable)
	void StopQuestTimer();
	UFUNCTION(BlueprintCallable)
	FTimespan CalculateTimeTaken() const;

	// クエスト中の戦闘情報
	UFUNCTION(BlueprintCallable)
	void AddHeadshotCount();
	UFUNCTION(BlueprintCallable)
	void AddBodyshotCount();
	UFUNCTION(BlueprintCallable)
	void AddDamageDealtTotal(int additionalDamage);
	UFUNCTION(BlueprintCallable)
	void AddDamageTakenTotal(int additionalDamage);
	UFUNCTION(BlueprintCallable)
	void AddDashCount();
	UFUNCTION(BlueprintCallable)
	void AddShotCount();
	
	UFUNCTION(BlueprintCallable)
	void ShowQuestResultWidget();
	UFUNCTION(BlueprintCallable)
	void ShowGameOverWidget();

private:

	UFUNCTION(BlueprintCallable)
	float CalculateAccuracy();
	TArray<TPair<FItemListStruct*, int>> CalculateItemDropList();
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPlayerInventoryComponent* playerInventory;
	
	UPROPERTY(VisibleAnywhere)
	FText questName;
	UPROPERTY(VisibleAnywhere)
	TArray<int> itemDropIDList;
	UPROPERTY(VisibleAnywhere)
	FDateTime startTime;
	UPROPERTY(VisibleAnywhere)
	FDateTime stopTime;
	UPROPERTY(VisibleAnywhere)
	FQuestStatStruct questStat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UQuestResultWidget> questResultWidgetClass = nullptr;
	UPROPERTY(VisibleAnywhere)
	UQuestResultWidget* questResultWidget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> gameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UUserWidget* gameOverWidget = nullptr;
};
