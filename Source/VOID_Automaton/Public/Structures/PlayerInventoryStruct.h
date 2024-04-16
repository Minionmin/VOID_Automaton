// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemListStruct.h"
#include "PlayerInventoryStruct.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct VOID_AUTOMATON_API FPlayerInventoryStruct : public FItemListStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 currentAmount = 0;
};
