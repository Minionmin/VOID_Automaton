// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structures/BuffListStruct.h"
#include "BuffListDataEntry.generated.h"

class UBuffLoadoutWidget;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UBuffListDataEntry : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitializeData(TSubclassOf<UBuff> buffClass, UTexture2D* buffIcon, FText buffName, int buffID, FText buffDescription,
		FText buffEffectDescription, float buffStrength, int32 buffWeight, UBuffLoadoutWidget* buffLoadoutWidgetRef);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBuffListStruct buffData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBuffLoadoutWidget* buffLoadoutWidget;
};
