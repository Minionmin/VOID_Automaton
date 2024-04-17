// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/BuffLoadoutWidget.h"

#include "Saves/MainGameInstance.h"
#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"
#include "Buffs/Buff.h"
#include "Components/BuffComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ListDataEntry/BuffListDataEntry.h"
#include "Managers/DataStorage.h"
#include "Saves/MainSaveGame.h"
#include "Structures/BuffListStruct.h"

#define MAXIMUM_BUFF_COUNT 4

void UBuffLoadoutWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// バフリストを初期化する
	InitializeBuffListView();
	
	// プレイヤーとプレイヤーのバフコンポネントのレファレンスを取得する
	auto playerActor = UGameplayStatics::GetPlayerCharacter(this, 0);
	if(playerActor)
	{
		playerCharacter = Cast<AVOIDCharacter>(playerActor);
		if(playerCharacter)
		{
			playerBuffComponent = playerCharacter->GetBuffComponent();
		}
	}

	// プレイヤーのバフ容量最大値に応じてテキストを設定する
	if(playerBuffComponent)
	{
		maxBuffCapacity = playerBuffComponent->GetBuffMaxCapacity(); // バフ追加時に重さを確認するために保存
		SetMaxBuffWeightText(maxBuffCapacity);
	}
	
	// ボタンがクリックされたときに呼び出される関数を設定する
	equippedBuffButton1->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnBuffButtonClicked);
	equippedBuffButton2->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnBuffButtonClicked);
	equippedBuffButton3->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnBuffButtonClicked);
	equippedBuffButton4->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnBuffButtonClicked);
	addBuffButton->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnAddBuffButtonClicked);
	confirmButton->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnConfirmButtonClicked);
	closeButton->OnClicked.AddDynamic(this, &UBuffLoadoutWidget::OnCloseButtonClicked);
}

void UBuffLoadoutWidget::InitializeBuffListView()
{
	// バフのデータをDataStorageに予めロードしておく
	UDataTable* buffListDataTable = nullptr;
	if(const auto dataStorage = UMasterUtilities::GetDataStorage(this))
	{
		buffListDataTable = dataStorage->LoadSyncDataTable(EDataTableType::BuffList);
	}
	
	// バフリストを初期化し、リストビューに追加する
	if(buffListDataTable)
	{
		for(const auto data : buffListDataTable->GetRowMap())
		{
			// 各データ行からクエストリスト構造体を取得する
			// *重要* data.ValueがFBuffListStruct型であることを確認してからキャストする
			FBuffListStruct* buffData = reinterpret_cast<FBuffListStruct*>(data.Value);
			if(buffData)
			{
				// バフリストのエントリーを作成し、データを初期化してからリストビューに追加する
				UBuffListDataEntry* buffDataEntry = NewObject<UBuffListDataEntry>(this);
				buffDataEntry->InitializeData(
					buffData->buffClass,
					buffData->buffIcon,
					buffData->buffName,
					buffData->buffID,
					buffData->buffDescription,
					buffData->buffEffectDescription,
					buffData->buffStrength,
					buffData->buffWeight,
					this
				);
				buffListView->AddItem(buffDataEntry);
			}
		}
	}
}

void UBuffLoadoutWidget::OnBuffButtonClicked()
{
	RemoveAllRegisteredBuff();
}

void UBuffLoadoutWidget::OnAddBuffButtonClicked()
{
	// バフの数が上限を超えないようにする
	if(registeredBuffs.Num() >= MAXIMUM_BUFF_COUNT) return;
	
	// 現在選択されたバフをプレイヤーのバフリストに追加する
	if(selectedBuffClass && playerBuffComponent)
	{
		// プレイヤーのバフリストに追加する前にバフの重さを確認する
		if(currentBuffWeight + selectedBuffWeight > maxBuffCapacity)
		{
			// バフの重さが上限を超える場合はバフを追加しない
			UE_LOG(LogTemp, Warning, TEXT("バフ容量が上限を超えました"));
			return;
		}
		
		// バフの重さを更新する
		currentBuffWeight += selectedBuffWeight;
		SetBuffWeightText(currentBuffWeight);
		
		// バフを生成し、プレイヤーのバフリストに追加する
		UBuff* newBuff = NewObject<UBuff>(this, selectedBuffClass);
		if(newBuff)
		{
			// バフのデータを更新する
			newBuff->InitializeBuff(selectedBuffID, selectedBuffName.ToString(), selectedBuffStrength);

			// 今後バフを外すために登録する
			registeredBuffs.Add(newBuff);

			// どのボタンのイメージアイコンを更新するかを決める
			const int currentRegisteredBuffCount = registeredBuffs.Num();
			
			// バフのイメージアイコンをセットする
			if(selectedBuffTexture)
			{
				switch (currentRegisteredBuffCount)
				{
				case 1:
					UUIUtilities::SetButtonStyle(equippedBuffButton1, selectedBuffTexture);
					break;
				case 2:
					UUIUtilities::SetButtonStyle(equippedBuffButton2, selectedBuffTexture);
					break;
				case 3:
					UUIUtilities::SetButtonStyle(equippedBuffButton3, selectedBuffTexture);
					break;
				case 4:
					UUIUtilities::SetButtonStyle(equippedBuffButton4, selectedBuffTexture);
					break;
				default:
					break;
				}
			}
		}
	
	}
}

void UBuffLoadoutWidget::RemoveAllRegisteredBuff()
{
	// バフを全部外して、バフのイメージアイコンを更新する
	if(playerBuffComponent && registeredBuffs.Num() > 0)
	{
		// 登録されたバフを全部外す
		registeredBuffs.Empty();

		// バフの重さをリセットする
		currentBuffWeight = 0;
		SetBuffWeightText(currentBuffWeight);
		
		// バフのイメージアイコンをリセットする
		UUIUtilities::SetButtonStyle(equippedBuffButton1, transparentTexture);
		UUIUtilities::SetButtonStyle(equippedBuffButton2, transparentTexture);
		UUIUtilities::SetButtonStyle(equippedBuffButton3, transparentTexture);
		UUIUtilities::SetButtonStyle(equippedBuffButton4, transparentTexture);
	}
}

void UBuffLoadoutWidget::OnConfirmButtonClicked()
{
	if(playerBuffComponent)
	{
		// 前に登録されたバフを全部外し、新しいバフを追加してからエフェクトを適用する
		playerBuffComponent->RemoveAllBuffs();
		playerBuffComponent->SetActiveBuffs(registeredBuffs);
		playerBuffComponent->ApplyAllBuffEffect();

		// プレイヤーのGameInstanceにバフデータを保存する
		const auto gameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(this));
		if(gameInstance)
		{
			// GameInstanceにバフリスト（Ubuffクラスの配列として）をShallow copyで保存
			gameInstance->SetBuffListInstance(playerBuffComponent->GetActiveBuffs());

			// ゲームファイルにもバフIDのDeep copyを保存
			auto saveData = gameInstance->GetSaveGame_Implementation()->GetPlayerSaveData();
			// SaveDataのバフIDを更新する（1. バフIDをクリアする 2. 登録されたバフIDを追加する）
			saveData.buffSaveDataIDs.Empty();
			for(const auto buff : registeredBuffs)
			{
				saveData.buffSaveDataIDs.Add(buff->GetBuffID());
			}

			// ゲームファイルを更新したSaveDataで上書きする
			gameInstance->SavePlayerStruct_Implementation(saveData);
			
			// プレイヤーに通知し、所持中のバフUIを更新させる
			gameInstance->OnBuffLoadoutSaved.Broadcast();
		}
	}

	// UIにある登録バフを解除、データのアンロード、ウィジェットを閉じて、ゲームモードに戻す
	CloseBuffWidget();
}

void UBuffLoadoutWidget::OnCloseButtonClicked()
{
	CloseBuffWidget();
}

void UBuffLoadoutWidget::CloseBuffWidget()
{
	// 登録されたバフがあれば全部外す
	RemoveAllRegisteredBuff();
	
	// Widgetをプレイヤーの画面から外す
	RemoveFromParent();

	// 使わなくなったデータテーブルをアンロード
	if(const auto dataStorage = UMasterUtilities::GetDataStorage(this))
	{
		dataStorage->UnloadDataTable(EDataTableType::BuffList);
	}
	
	// プレイヤーのコントローラーの入力モードをゲームモードに戻す
	UUIUtilities::SetFocusInGame(this);
}

void UBuffLoadoutWidget::SetButtonStyle(UButton* button, UTexture2D* buttonTexture)
{
	FSlateBrush buttonBrush;
	buttonBrush.SetResourceObject(buttonTexture);
	buttonBrush.ImageSize = FVector2D(buttonTexture->GetSizeX(),
		buttonTexture->GetSizeY());
	
	FButtonStyle ButtonStyle = button->GetStyle();
	ButtonStyle.SetNormal(buttonBrush);
	ButtonStyle.SetHovered(buttonBrush);
	ButtonStyle.SetPressed(buttonBrush);
	button->SetStyle(ButtonStyle);
}

void UBuffLoadoutWidget::SetBuffImage(UTexture2D* newBuffImage)
{
	buffImage->SetBrushFromTexture(newBuffImage);
}

void UBuffLoadoutWidget::SetBuffDescription(FText newBuffDesc)
{
	buffDescription->SetText(newBuffDesc);
}

void UBuffLoadoutWidget::SetBuffEffectDescription(FText newBuffEffectDesc)
{
	buffEffectDescription->SetText(newBuffEffectDesc);
}

void UBuffLoadoutWidget::SetBuffWeightText(int32 newBuffWeight)
{
	buffWeightText->SetText(FText::Format(FText::FromString("{0} / {1}"),
	FText::AsNumber(newBuffWeight),
	FText::AsNumber(playerBuffComponent->GetBuffMaxCapacity())));
}

void UBuffLoadoutWidget::SetMaxBuffWeightText(int32 newMaxWeight)
{
	buffWeightText->SetText(FText::Format(FText::FromString("{0} / {1}"),
		FText::AsNumber(currentBuffWeight),
		FText::AsNumber(newMaxWeight)));
}

void UBuffLoadoutWidget::SetSelectedBuffClass(TSubclassOf<UBuff> newBuffClass)
{
	selectedBuffClass = newBuffClass;
}

void UBuffLoadoutWidget::SetSelectedBuffID(int newBuffID)
{
	selectedBuffID = newBuffID;
}

void UBuffLoadoutWidget::SetSelectedBuffName(FText newBuffName)
{
	selectedBuffName = newBuffName;
}

void UBuffLoadoutWidget::SetSelectedBuffStrength(float newBuffStrength)
{
	selectedBuffStrength = newBuffStrength;
}

void UBuffLoadoutWidget::SetSelectedBuffWeight(float newBuffWeight)
{
	selectedBuffWeight = newBuffWeight;
}

void UBuffLoadoutWidget::SetSelectedBuffTexture(UTexture2D* newBuffTexture)
{
	selectedBuffTexture = newBuffTexture;
}
