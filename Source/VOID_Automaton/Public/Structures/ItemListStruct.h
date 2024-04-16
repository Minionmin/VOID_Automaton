// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemListStruct.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "ItemRarity"))
enum class EItemRarity : uint8
{
	EIR_None,
	EIR_Common,
	EIR_Uncommon,
	EIR_Rare,
	EIR_SuperRare,
	EIR_Epic,
	EIR_Legendary,
};

/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct VOID_AUTOMATON_API FItemListStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 itemID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* itemIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText itemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity itemRarity = EItemRarity::EIR_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText itemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dropRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maximumDropAmount = 0;
};
