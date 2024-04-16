// Fill out your copyright notice in the Description page of Project Settings.

/*
 * 本のリストとして使用するクラス
 * NativeOnListItemObjectSet関数は、データエントリーがメインWidgetにAddItem関数により
 * 追加されるたびに呼び出される
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Structures/QuestListStruct.h"
#include "QuestListViewEntryWidget.generated.h"

class UTextBlock;
class ULevelSelectorWidget;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UQuestListViewEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	
protected:
	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	// questButtonがクリックされたときに呼び出される関数
	UFUNCTION()
	void OnQuestButtonClicked();
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* questButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* questNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* questTypeIcon;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FQuestListStruct questData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ULevelSelectorWidget* levelSelectorWidget;
};
