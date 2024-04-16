// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/BuffListViewEntryWidget.h"

#include "Buffs/Buff.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "HUDs/BuffLoadoutWidget.h"
#include "ListDataEntry/BuffListDataEntry.h"

void UBuffListViewEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	buffButton->OnClicked.AddDynamic(this, &UBuffListViewEntryWidget::OnBuffButtonClicked);
}

void UBuffListViewEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// 任意のデータタイプにキャストして取得する （ここではバフリスト用のエントリー「UBuffListDataEntry」）
	auto buffDataEntry = Cast<UBuffListDataEntry>(ListItemObject);
	if(buffDataEntry)
	{
		// このUIWidgetのデータを更新する
		buffIcon->SetBrushFromTexture(buffDataEntry->buffData.buffIcon);
		buffNameText->SetText(buffDataEntry->buffData.buffName);
		buffWeightText->SetText(FText::AsNumber(buffDataEntry->buffData.buffWeight)); // int32 -> FText

		// ここではデータを更新するだけで、UIの更新はメインUI側で行う（BuffLoadoutWidget）
		buffClass = buffDataEntry->buffData.buffClass;
		buffImageTexture = buffDataEntry->buffData.buffIcon;
		buffID = buffDataEntry->buffData.buffID;
		buffName = buffDataEntry->buffData.buffName;
		buffStrength = buffDataEntry->buffData.buffStrength;
		buffWeight = buffDataEntry->buffData.buffWeight;
		buffDescText = buffDataEntry->buffData.buffDescription;
		buffEffectDescText = buffDataEntry->buffData.buffEffectDescription;

		// メインUI（BuffLoadoutWidget）のレファレンスを取得する
		buffLoadoutWidget = buffDataEntry->buffLoadoutWidget;
	}
}

void UBuffListViewEntryWidget::OnBuffButtonClicked()
{
	if(!buffLoadoutWidget) return;
	
	// メインUI（BuffLoadoutWidget）にデータを渡す
	if(buffClass)
	{
		// バフのクラスを選択されたバフと一致させる
		buffLoadoutWidget->SetSelectedBuffClass(buffClass);
	}

	if(buffImageTexture)
	{
		// バフの画像を選択されたバフの画像と一致させる
		buffLoadoutWidget->SetBuffImage(buffImageTexture);
	}

	buffLoadoutWidget->SetBuffDescription(buffDescText);
	buffLoadoutWidget->SetBuffEffectDescription(buffEffectDescText);
	buffLoadoutWidget->SetSelectedBuffID(buffID);
	buffLoadoutWidget->SetSelectedBuffName(buffName);
	buffLoadoutWidget->SetSelectedBuffStrength(buffStrength);
	buffLoadoutWidget->SetSelectedBuffWeight(buffWeight);
	buffLoadoutWidget->SetSelectedBuffTexture(buffImageTexture);
}