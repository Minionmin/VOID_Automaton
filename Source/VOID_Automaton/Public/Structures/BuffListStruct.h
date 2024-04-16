// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffListStruct.generated.h"

class UBuff;

USTRUCT(Blueprintable, BlueprintType)
struct VOID_AUTOMATON_API FBuffListStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBuff> buffClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* buffIcon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText buffName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName buffID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText buffDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText buffEffectDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float buffStrength = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 buffWeight = 0;
};
