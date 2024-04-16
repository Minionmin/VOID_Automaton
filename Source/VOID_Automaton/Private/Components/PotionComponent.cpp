// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PotionComponent.h"

#include "Items/Potion.h"

UPotionComponent::UPotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UPotionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// インベントリーにポーションを追加する関数
void UPotionComponent::AddPotion(TSubclassOf<UPotion> potionType, int quantity)
{
	// ポーションの種類が既にインベントリーに存在するか確認
	for(auto& potionInfo: potionInventory)
	{
		if(potionInfo.potionType == potionType)
		{
			// 既に存在する場合は数量を増やす
			potionInfo.quantity += quantity;
			return;
		}
	}
	
	// ポーションの種類と数量をインベントリーに追加
	potionInventory.Add(FPotionInfo(potionType, quantity));
}

void UPotionComponent::UsePotionSlot(int potionSlot, AVOIDCharacter* usingCharacter)
{
	// ポーションスロットが有効なインデックスであるか確認
	if(potionInventory.IsValidIndex(potionSlot))
	{
		auto& potionInfo = potionInventory[potionSlot];

		// ポーションを使用する
		UsePotion(potionInfo, usingCharacter);
	}
}

void UPotionComponent::UsePotion(FPotionInfo& potionInfo, AVOIDCharacter* usingCharacter)
{
	if(potionInfo.quantity <= 0) return;
	
	// ポーション種類によって異なる効果を適用
	potionInfo.potionType.GetDefaultObject()->Interact_Implementation(usingCharacter);

	// ポーションの数量を減らす
	potionInfo.quantity--;

	// ポーションが使用されたことを通知（プレイヤーのアニメーション）
	OnPotionUsed.Broadcast();
	// PlayerInfoUIにポーションの数量が変更されたことを通知
	OnPotionCapacityChanged.Broadcast(GetPotionCapacityArray());
}

TArray<UTexture2D*> UPotionComponent::GetPotionTextures()
{
	TArray<UTexture2D*> potionTextures;
	for(const auto& potionInfo: potionInventory)
	{
		potionTextures.Add(potionInfo.potionType.GetDefaultObject()->GetPotionTexture2D());
	}
	
	return potionTextures;
}

TArray<int> UPotionComponent::GetPotionCapacityArray()
{
	TArray<int> potionCapacityArray;
	for(const auto& potionInfo: potionInventory)
	{
		potionCapacityArray.Add(potionInfo.quantity);
	}
	
	return potionCapacityArray;
}

