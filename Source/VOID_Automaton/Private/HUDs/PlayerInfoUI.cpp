// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/PlayerInfoUI.h"

#include "MyCustomUnrealLibrary.h"
#include "Buffs/Buff.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Managers/DataStorage.h"
#include "Saves/MainGameInstance.h"
#include "Saves/MainSaveGame.h"
#include "Structures/BuffListStruct.h"

#define HEALTH_BAR_MIN_WIDTH 300.f
#define HEALTH_BAR_MAX_WIDTH 800.f
#define STAMINA_BAR_MIN_WIDTH 300.f
#define STAMINA_BAR_MAX_WIDTH 500.f
#define MAX_POTION_CAPACITY 4

void UPlayerInfoUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	// GameInstanceのイベントに登録
	gameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if(gameInstance)
	{
		gameInstance->OnBuffLoadoutSaved.AddDynamic(this, &UPlayerInfoUI::UpdateBuffImages);
	}
}

void UPlayerInfoUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerInfoUI::SetHealthBarPercentage(float value)
{
	Super::SetHealthBarPercentage(value);
}

void UPlayerInfoUI::SetStaminaBarPercentage(float value)
{
	// ** Tween 演出 **
	// インプット値が現在のスタミナバーの値より大きい場合、代わりにTweenのスタミナバーの値を増加させる
	if(value > staminaProgressBar->GetPercent())
	{
		SetTweenStaminaBarPercentage(value);
	}
	// 逆に、インプット値が現在のスタミナバーの値より小さい場合、本のスタミナバーの値を減少させTweenのスタミナバーの値を合わせる
	else if(value < staminaProgressBar->GetPercent())
	{
		staminaProgressBar->SetPercent(value);
		SetTweenStaminaBarPercentage(value);
	}
}

void UPlayerInfoUI::SetTweenStaminaBarPercentage(float value)
{
	tweenStaminaProgressBar->SetPercent(value);
}

// プレイヤーの体力上限に応じて体力バーのサイズを変更（イベントに登録用）
void UPlayerInfoUI::SetHealthBarSize(float maxHealth)
{
	const float desiredWidth = FMath::Clamp(maxHealth * widthPerMaxHealth, HEALTH_BAR_MIN_WIDTH, HEALTH_BAR_MAX_WIDTH);

	healthSizeBox->SetWidthOverride(desiredWidth);
	tweenHealthSizeBox->SetWidthOverride(desiredWidth);
	healthBackgroundBox->SetWidthOverride(desiredWidth);
}

// プレイヤーのスタミナ上限に応じてスタミナバーのサイズを変更（イベントに登録用）
void UPlayerInfoUI::SetStaminaBarSize(float maxStamina)
{
	const float desiredWidth = FMath::Clamp(maxStamina * widthPerMaxStamina, STAMINA_BAR_MIN_WIDTH, STAMINA_BAR_MAX_WIDTH);

	staminaSizeBox->SetWidthOverride(desiredWidth);
	tweenStaminaSizeBox->SetWidthOverride(desiredWidth);
	staminaBackgroundBox->SetWidthOverride(desiredWidth);
}

void UPlayerInfoUI::SetAmmoLeft(int32 ammoLeft)
{
	const FString ammoString = FString::FromInt(ammoLeft);
	ammoText->SetText(FText::FromString(ammoString));
}

void UPlayerInfoUI::TweenBars(float InDeltaTime)
{
	Super::TweenBars(InDeltaTime);
	
	// Tweenのスタミナバーが現在のスタミナバーと一致していない場合、Tweenのスタミナバーを更新
	if(!IsTweenStaminaBarMatched())
	{
		// Tween スタミナバー
		UUIUtilities::InterpProgressBar(staminaProgressBar, tweenStaminaProgressBar->GetPercent(), InDeltaTime, staminaBarInterpSpeed);
	}
}

bool UPlayerInfoUI::IsTweenStaminaBarMatched() const
{
	return tweenStaminaProgressBar->GetPercent() == staminaProgressBar->GetPercent();
}

void UPlayerInfoUI::UpdateBuffImages()
{
	// バフデータテーブルを取得
	const auto dataStorage = UMasterUtilities::GetDataStorage(this);
	const UDataTable* buffTable = nullptr;
	if(dataStorage)
	{
		buffTable = dataStorage->LoadSyncDataTable(EDataTableType::BuffList);
	}
	if(!buffTable) return;
	
	// 上手いこと配列として扱う
	UImage* buffIcons[] = { buffIcon1, buffIcon2, buffIcon3, buffIcon4 };
	
	// バフのアイコンを更新
	// まずバフのアイコンをクリア
	if(transparentTexture)
	{
		for(int i = 0; i < 4; i++)
		{
			buffIcons[i]->SetBrushFromTexture(transparentTexture);
		}
	}
	
	// GameInstanceが現在持っているバフリストを取得
	const auto gameInstace = Cast<UMainGameInstance>(GetGameInstance());
	if(!gameInstace) return;
	const auto buffList = gameInstace->GetBuffListInstance();
		
	for(int32 i = 0; i < buffList.Num(); i++)
	{
		// バフデータテーブルからバフのアイコンを取得（RowNameで検索）
		UTexture2D* buffIcon = nullptr;
		FName rowName = FName(*FString::FromInt(buffList[i]->GetBuffID()));
		FBuffListStruct* buffData = buffTable->FindRow<FBuffListStruct>(rowName, TEXT("Buff ID not found"));
		if(buffData)
		{
			buffIcon = buffData->buffIcon;
		}
			
		// バフが存在しない場合、次のバフをチェック
		if(!buffIcon)
		{
			buffIcon = transparentTexture;
		}
			
		// バフのアイコンをセット
		buffIcons[i]->SetBrushFromTexture(buffIcon);
	}

	// バフデータテーブルはもう不要なのでアンロード
	dataStorage->UnloadDataTable(EDataTableType::BuffList);
}

void UPlayerInfoUI::UpdatePotionUIs(const TArray<UTexture2D*>& potionTextures, const TArray<int>& potionCapacityArray)
{
	// 上手いこと配列として扱う
	TArray potionIcons = { potionIcon1, potionIcon2, potionIcon3, potionIcon4 };
	TArray potionCapacityImages = { potionCapacityImage1, potionCapacityImage2, potionCapacityImage3, potionCapacityImage4 };
	TArray potionCapacityTexts = { potionCapacityText1, potionCapacityText2, potionCapacityText3, potionCapacityText4 };

	// まずポーションUIをクリア
	if(transparentTexture)
	{
		for(int i = 0; i < MAX_POTION_CAPACITY; i++)
		{
			potionIcons[i]->SetBrushFromTexture(transparentTexture);
			potionCapacityImages[i]->SetBrushFromTexture(transparentTexture);
			potionCapacityTexts[i]->SetText(FText::FromString(""));
		}
	}

	// ポーションのアイコン・数量を更新
	for(auto i = 0; i < potionTextures.Num(); i++)
	{
		potionIcons[i]->SetBrushFromTexture(potionTextures[i]);
		if(potionCapacityTexture)
		{
			potionCapacityImages[i]->SetBrushFromTexture(potionCapacityTexture);	
		}
		UUIUtilities::UpdateTextBlock(potionCapacityTexts[i], potionCapacityArray[i]);
	}
}

void UPlayerInfoUI::UpdatePotionCapacity(const TArray<int>& potionCapacityArray)
{
	TArray potionCapacityTexts = { potionCapacityText1, potionCapacityText2, potionCapacityText3, potionCapacityText4 };

	for(auto i = 0; i < potionCapacityArray.Num(); i++)
	{
		UUIUtilities::UpdateTextBlock(potionCapacityTexts[i], potionCapacityArray[i]);
	}
}
