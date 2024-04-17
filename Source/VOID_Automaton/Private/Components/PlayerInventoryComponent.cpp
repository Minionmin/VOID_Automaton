// Fill out your copyright notice in the Description page of Project Settings.


#include "Components\PlayerInventoryComponent.h"

#include "MyCustomUnrealLibrary.h"


UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(const auto questManager = UMasterUtilities::GetQuestManager(this))
	{
		// クエストの開始時にPlayerのインベントリを保存するように設定
		questManager->OnQuestStarted.AddDynamic(this, &UPlayerInventoryComponent::SavePlayerInventory);
		// プレイヤーがレベルにロードされる時にプレイヤーのインベントリをロードするように設定
		questManager->LoadPlayerInventory(this);
	}
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerInventoryComponent::AddItem(int itemIDToCheck, int amountToAdd)
{
	if(HasItem(itemIDToCheck))
	{
		itemInventory[itemIDToCheck] += amountToAdd;
	}
	else
	{
		itemInventory.Add(itemIDToCheck, amountToAdd);
	}
}

void UPlayerInventoryComponent::RemoveItem(const FPlayerInventoryStruct& itemToRemove)
{
	itemInventory.Remove(itemToRemove.itemID);
}

bool UPlayerInventoryComponent::HasItem(int itemIDToCheck)
{
	return itemInventory.Contains(itemIDToCheck) && itemInventory[itemIDToCheck] > 0;
}

int UPlayerInventoryComponent::GetItemAmount(int itemIDToCheck)
{
	// アイテムIDを比較して、所持数を返す
	if(HasItem(itemIDToCheck))
	{
		return itemInventory[itemIDToCheck];
	}
	
	return 0;
}

TArray<int> UPlayerInventoryComponent::GetItemIDList() const
{
	TArray<int> itemIDList;
	for(const auto& item : itemInventory)
	{
		itemIDList.Add(item.Key);
	}
	return itemIDList;
}

void UPlayerInventoryComponent::SavePlayerInventory()
{
	if(const auto questManager = UMasterUtilities::GetQuestManager(this))
	{
		questManager->SavePlayerInventory(itemInventory);
	}
}