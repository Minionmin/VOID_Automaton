// Fill out your copyright notice in the Description page of Project Settings.


#include "Saves/MainGameInstance.h"

#include "Buffs/Buff.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AudioManager.h"
#include "Managers/DataStorage.h"
#include "Managers/QuestManager.h"
#include "Saves/MainSaveGame.h"
#include "Structures/BuffListStruct.h"

void UMainGameInstance::Init()
{
	Super::Init();
	
	// この時点ではゲームにおいて何も（マップなど）初期化されていない
	// このGameInstanceのセーブデータに前回のセーブデータをロードする
	LoadGameData_Implementation(false);

	// シングルトン
	InitializeSingleton();
}

// このGameInstanceのセーブデータをゲームファイルからロード（GameInstanceのInitで初めてコールされる）
void UMainGameInstance::LoadGameData_Implementation(bool bAsync)
{
	bool bSaveGameExisted = UGameplayStatics::DoesSaveGameExist(saveSlotName, 0);
	if (bSaveGameExisted)
	{
		// ロードが非同期に行われる
		// 非同期に行われるため、成功時と失敗時のコールバックを設定
		if(bAsync)
		{
			FAsyncLoadGameFromSlotDelegate asyncLoadDelegate;
			asyncLoadDelegate.BindUFunction(this, "OnAsyncLoadGameSuccess");
			UGameplayStatics::AsyncLoadGameFromSlot(saveSlotName, 0, asyncLoadDelegate);
		}
		// ロードが即時に行われる
		else
		{
			mainSaveGame = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(saveSlotName, 0));
		}
	}
	else
	{
		mainSaveGame = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	}
}

// このGameInstanceのセーブデータをゲームファイルにセーブ（SavePlayerStruct_Implementationからコールください）
void UMainGameInstance::SaveGameData_Implementation(bool bAsync)
{
	if(bAsync)
	{
		UGameplayStatics::AsyncSaveGameToSlot(mainSaveGame, saveSlotName, 0);
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(mainSaveGame, saveSlotName, 0);
	}
}

// Initでゲームファイルから取得したこのGameInstanceのセーブデータを返す
UMainSaveGame* UMainGameInstance::GetSaveGame_Implementation()
{
	return mainSaveGame;
}

// このGameInstanceのセーブデータを更新して、ゲームファイルにもセーブ
void UMainGameInstance::SavePlayerStruct_Implementation(FPlayerSaveStruct playerSaveData, bool bAsync)
{
	// このGameInstanceのセーブデータを更新
	mainSaveGame->SetPlayerSaveData(playerSaveData);
	// ゲームファイルにもセーブデータをセーブ
	SaveGameData_Implementation(bAsync);
}

// Asyncロードが成功した時にGameInstanceのセーブデータを更新
void UMainGameInstance::OnAsyncLoadGameSuccess(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	mainSaveGame = Cast<UMainSaveGame>(SaveGame);

	// 成功した時の通知
	OnGameLoaded.Broadcast();
}

UAudioManager* UMainGameInstance::GetAudioManager_Implementation()
{
	return AudioManagerInstance;
}

UDataStorage* UMainGameInstance::GetDataStorage_Implementation()
{
	return DataStorageInstance;
}

UQuestManager* UMainGameInstance::GetQuestManager_Implementation()
{
	return QuestManagerInstance;
}

// プレイヤーのバフリストをGameInstanceから取得
TArray<UBuff*> UMainGameInstance::GetBuffListInstance() const
{
	return playerBuffList;
}

// プレイヤーのバフリストをGameInstanceでセット（Shallow copy）
void UMainGameInstance::SetBuffListInstance(TArray<UBuff*> buffList)
{
	playerBuffList = buffList;
}

void UMainGameInstance::InitializeSingleton()
{
	// オーディオマネージャのインスタンスを取得（初期化）
	AudioManagerInstance = NewObject<UAudioManager>(this, audioManagerClass);
	AudioManagerInstance->SetMasterVolume(mainSaveGame->GetPlayerSaveData().masterVolume);
	AudioManagerInstance->SetBGMVolume(mainSaveGame->GetPlayerSaveData().bgmVolume);
	AudioManagerInstance->SetSFXVolume(mainSaveGame->GetPlayerSaveData().sfxVolume);
	// DataStorageのインスタンスを取得（初期化）+ バフリストの初期化
	DataStorageInstance = NewObject<UDataStorage>(this, dataStorageClass);
	// クエストマネージャーのインスタンスを取得（初期化）
	QuestManagerInstance = NewObject<UQuestManager>(this, questManagerClass);
}

void UMainGameInstance::LoadBuffListFromSaveFile()
{
	// バフリストのデータテーブルを取得
	auto buffDataIDs = mainSaveGame->GetPlayerSaveData().buffSaveDataIDs;
	const auto buffListDataTable = DataStorageInstance->LoadSyncDataTable(EDataTableType::BuffList);
	
	if(buffListDataTable)
	{
		if(playerBuffList.Num() > 0)
		{
			playerBuffList.Empty();
		}
		for(auto i = 0; i < buffDataIDs.Num(); i++)
		{
			// バフデータテーブルからバフのアイコンを取得（RowNameで検索）
			const FName rowName = FName(*FString::FromInt(buffDataIDs[i]));
			FBuffListStruct* buffData = buffListDataTable->FindRow<FBuffListStruct>(rowName, TEXT("Buff ID not found"));
			if(buffData)
			{
				// セーブファイルからバフリストを再構築（バフFNameで検索）
				UBuff* buff = NewObject<UBuff>(this, buffData->buffClass);
				buff->InitializeBuff(buffDataIDs[i], buffData->buffName.ToString(), buffData->buffStrength);
				playerBuffList.Add(buff);
			}
		}

		// バフデータテーブルはもう使わないので削除
		DataStorageInstance->UnloadDataTable(EDataTableType::BuffList);
	}
}