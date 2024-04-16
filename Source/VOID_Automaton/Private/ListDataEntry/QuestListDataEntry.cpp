// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDataEntry/QuestListDataEntry.h"

void UQuestListDataEntry::InitializeData(
	UTexture2D* questIcon,
	FText questName,
	UTexture2D* questImage,
	FName levelName,
	const TArray<int>& dropItemIDList,
	ULevelSelectorWidget* levelSelectorWidgeRef
	)
{
	questData.questIcon = questIcon;
	questData.questName = questName;
	questData.questImage = questImage;
	questData.levelName = levelName;
	questData.dropItemIDList = dropItemIDList;
	levelSelectorWidget = levelSelectorWidgeRef;
}
