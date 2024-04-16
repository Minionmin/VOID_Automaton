// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/ItemDropListViewEntryWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HUDs/ItemHoverWidget.h"
#include "ListDataEntry/ItemDropListDataEntry.h"

void UItemDropListViewEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	itemButton->OnHovered.AddDynamic(this, &UItemDropListViewEntryWidget::OnItemHovered);
	itemButton->OnUnhovered.AddDynamic(this, &UItemDropListViewEntryWidget::OnItemUnhovered);
}

void UItemDropListViewEntryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// ホバーしたアイテムのUIが残っている場合は削除
	OnItemUnhovered();
}

void UItemDropListViewEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	// 任意のデータタイプにキャストして取得する （ここではクエストリスト用のエントリー「UQuestListDataEntry」）
	auto itemDataEntry = Cast<UItemDropListDataEntry>(ListItemObject);
	if(itemDataEntry)
	{
		// このUIWidgetのデータを更新する
		itemIcon->SetBrushFromTexture(itemDataEntry->itemData.itemIcon);
		itemButton->SetBackgroundColor(GetColorBasedOnRarity(itemDataEntry->itemData.itemRarity));
		if(itemDataEntry->itemAmount > 0)
		{
			const FString itemAmountString = "x" + FString::FromInt(itemDataEntry->itemAmount);
			itemAmountText->SetText(FText::FromString(itemAmountString));
		}
		else
		{
			itemAmountText->SetText(FText::FromString(""));
		}
		
		// アイテムボタンをホバーしたときに表示されるUIの設定
		// クエスト開始時にQuestManagerに渡すデータを設定する
		itemData.itemID = itemDataEntry->itemData.itemID;
		itemData.itemName = itemDataEntry->itemData.itemName;
		itemData.itemRarity = itemDataEntry->itemData.itemRarity;
		itemData.itemDescription = itemDataEntry->itemData.itemDescription;
		itemData.itemIcon = itemDataEntry->itemData.itemIcon;
		itemData.dropRate = itemDataEntry->itemData.dropRate;
	}
}

void UItemDropListViewEntryWidget::OnItemHovered()
{
	if(itemHoverWidgetClass)
	{
		if(!currentItemHoverWidget)
		{
			currentItemHoverWidget = CreateWidget<UItemHoverWidget>(GetWorld(), itemHoverWidgetClass);
			if(currentItemHoverWidget)
			{
				currentItemHoverWidget->AddToViewport();
				
				// ホバーしたアイテムのデータをセット
				currentItemHoverWidget->SetItemData(itemData.itemIcon, GetColorBasedOnRarity(itemData.itemRarity),
					itemData.itemName, itemData.itemDescription);

				// ホバーしたアイテムのUIを指定のサイズに設定
				currentItemHoverWidget->SetDesiredSizeInViewport(itemHoverWidgetSize);
				
				// ホバーしたアイテムのUIをマウスカーソルの位置に表示
				FVector2D mousePosition;
				GetWorld()->GetFirstPlayerController()->GetMousePosition(mousePosition.X, mousePosition.Y);
				currentItemHoverWidget->SetPositionInViewport(mousePosition  - itemHoverWidgetOffset);
			}
		}
	}
}

void UItemDropListViewEntryWidget::OnItemUnhovered()
{
	if(currentItemHoverWidget)
	{
		currentItemHoverWidget->RemoveFromParent();
		currentItemHoverWidget = nullptr;
	}
}

FLinearColor UItemDropListViewEntryWidget::GetColorBasedOnRarity(EItemRarity itemRarity)
{
	return *rarityColorMap.Find(itemRarity);
}
