// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/ItemHoverWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemHoverWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemHoverWidget::SetItemData(UTexture2D* itemTexture, FLinearColor rarityColor, FText itemName, FText itemDesc)
{
	itemIcon->SetBrushFromTexture(itemTexture);
	rarityImage->SetColorAndOpacity(rarityColor);
	itemNameText->SetText(itemName);
	itemDescText->SetText(itemDesc);
}
