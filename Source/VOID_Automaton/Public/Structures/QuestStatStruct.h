// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestStatStruct.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct VOID_AUTOMATON_API FQuestStatStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int headshotCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int bodyshotCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int totalShotCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int damageDealtTotal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int damageTakenTotal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int dashCount = 0;
};
