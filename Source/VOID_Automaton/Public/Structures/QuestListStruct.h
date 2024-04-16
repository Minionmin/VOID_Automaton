// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestListStruct.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct VOID_AUTOMATON_API FQuestListStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* questIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText questName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* questImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName levelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> dropItemIDList;
};
