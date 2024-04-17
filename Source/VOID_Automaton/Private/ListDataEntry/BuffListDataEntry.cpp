// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDataEntry/BuffListDataEntry.h"

void UBuffListDataEntry::InitializeData(
	TSubclassOf<UBuff> buffClass,
	UTexture2D* buffIcon,
	FText buffName,
	int buffID,
	FText buffDescription,
	FText buffEffectDescription,
	float buffStrength,
	int32 buffWeight,
	UBuffLoadoutWidget* buffLoadoutWidgetRef
	)
{
	buffData.buffClass = buffClass;
	buffData.buffIcon = buffIcon;
	buffData.buffName = buffName;
	buffData.buffID = buffID;
	buffData.buffDescription = buffDescription;
	buffData.buffEffectDescription = buffEffectDescription;
	buffData.buffStrength = buffStrength;
	buffData.buffWeight = buffWeight;
	buffLoadoutWidget = buffLoadoutWidgetRef;
}
