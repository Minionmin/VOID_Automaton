// Fill out your copyright notice in the Description page of Project Settings.

/*
 * クエストリストのメインWidget
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelectorWidget.generated.h"

class UImage;
class UButton;
class UListView;
class UDataTable;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API ULevelSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeQuestList();
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonClicked();
	UFUNCTION()
	void OnCloseButtonClicked();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* startButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* closeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* questImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UListView* questListView;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UListView* itemDropListView;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName levelName;

	// QuestManagerに渡す情報
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText questName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<int> itemDropIDList;
	
	
	UPROPERTY(VisibleAnywhere)
	UDataTable* itemDataTable = nullptr;

public:

	UFUNCTION(BlueprintCallable)
	void SetLevelName(FName newLevelName);
	UFUNCTION(BlueprintCallable)
	void SetQuestName(FText newQuestName);
	UFUNCTION(BlueprintCallable)
	void SetQuestImage(UTexture2D* newQuestImage);
	UFUNCTION(BlueprintCallable)
	void SetItemDropList(const TArray<int>& itemDropIDArray);
};
