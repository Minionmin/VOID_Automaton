// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/QuestListViewEntryWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HUDs/LevelSelectorWidget.h"
#include "ListDataEntry/QuestListDataEntry.h"

void UQuestListViewEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	questButton->OnClicked.AddDynamic(this, &UQuestListViewEntryWidget::OnQuestButtonClicked);
}

void UQuestListViewEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// 任意のデータタイプにキャストして取得する （ここではクエストリスト用のエントリー「UQuestListDataEntry」）
	auto questDataEntry = Cast<UQuestListDataEntry>(ListItemObject);
	if(questDataEntry)
	{
		// このUIWidgetのデータを更新する
		questTypeIcon->SetBrushFromTexture(questDataEntry->questData.questIcon);
		questNameText->SetText(questDataEntry->questData.questName);

		// メインUI（LevelSelectorWidget）で更新される予定のデータを用意する
		questData = questDataEntry->questData; // shallow copy
		levelSelectorWidget = questDataEntry->levelSelectorWidget;
	}
}

void UQuestListViewEntryWidget::OnQuestButtonClicked()
{
	if(!levelSelectorWidget) return;
	
	// メインUI（LevelSelectorWidget）にデータを渡す
	if(questData.questImage)
	{
		// クエストのイメージを選択されたクエストと一致させる
		levelSelectorWidget->SetQuestImage(questData.questImage);
	}
	
	// クエストのレベル名を選択されたクエストと一致させる
	levelSelectorWidget->SetLevelName(questData.levelName);
	// クエストの名前を選択されたクエストと一致させる
	levelSelectorWidget->SetQuestName(questData.questName);
	// クエストのドロップアイテムリストを選択されたクエストと一致させる
	levelSelectorWidget->SetItemDropList(questData.dropItemIDList);
	
}
