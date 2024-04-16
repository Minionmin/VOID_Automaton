// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Structures/ItemListStruct.h"
#include "ItemDropListViewEntryWidget.generated.h"

class UItemHoverWidget;
enum class EItemPickRarity : uint8;
class UButton;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UItemDropListViewEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	// アイテムがホバーされたときに呼び出される関数
	UFUNCTION()
	void OnItemHovered();
	UFUNCTION()
	void OnItemUnhovered();
	UFUNCTION()
	FLinearColor GetColorBasedOnRarity(EItemRarity itemRarity);
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* itemButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* itemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* itemAmountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EItemRarity, FLinearColor> rarityColorMap;
	

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	FItemListStruct itemData;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemHoverWidget> itemHoverWidgetClass;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UItemHoverWidget* currentItemHoverWidget;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector2D itemHoverWidgetSize = FVector2D(500.0f, 500.0f);
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector2D itemHoverWidgetOffset = FVector2D(100.f, 150.0f);
};
