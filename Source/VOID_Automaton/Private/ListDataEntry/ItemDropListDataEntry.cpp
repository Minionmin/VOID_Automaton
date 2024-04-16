// Fill out your copyright notice in the Description page of Project Settings.


#include "ListDataEntry/ItemDropListDataEntry.h"

void UItemDropListDataEntry::InitializeData(int itemID, UTexture2D* itemIcon, FText itemName, EItemRarity itemRarity, FText itemDesc, float itemDropRate, int newItemAmount)
{
	itemData.itemID = itemID;
	itemData.itemIcon = itemIcon;
	itemData.itemName = itemName;
	itemData.itemRarity = itemRarity;
	itemData.itemDescription = itemDesc;
	itemData.dropRate = itemDropRate;
	itemAmount = newItemAmount;
}
