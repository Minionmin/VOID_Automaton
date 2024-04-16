// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/HasSaveGame.h"
#include "Interfaces/ManageSingleton.h"
#include "MainGameInstance.generated.h"

class UQuestManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffLoadoutSaved);

class UAudioManager;
class UDataStorage;
class UMainSaveGame;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMainGameInstance : public UGameInstance, public IHasSaveGame, public IManageSingleton
{
	GENERATED_BODY()

public:
	
	virtual void Init() override;

	void InitializeSingleton();
	void LoadBuffListFromSaveFile();

	/*
		イベント
	*/
	FOnGameLoaded OnGameLoaded;
	FOnBuffLoadoutSaved OnBuffLoadoutSaved;
	UFUNCTION()
	void OnAsyncLoadGameSuccess(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
	
	/*
		IHasSaveGame インタフェース
	*/
	virtual void LoadGameData_Implementation(bool bAsync = false) override;
	virtual void SaveGameData_Implementation(bool bAsync = false) override;
	virtual UMainSaveGame* GetSaveGame_Implementation() override;
	virtual void SavePlayerStruct_Implementation(FPlayerSaveStruct playerSaveData, bool bAsync = false) override;
	virtual void SetNewGameStarted_Implementation(bool newVal) override { bNewGameStarted = newVal; }
	virtual bool GetNewGameStarted_Implementation() const override { return bNewGameStarted; }
	virtual void SetMainMenuContinue_Implementation(bool newVal) override { bMainMenuContinue = newVal; }
	virtual bool GetMainMenuContinue_Implementation() const override { return bMainMenuContinue; }
	virtual void SetDataNextLevel_Implementation(bool newVal) override { bDataNextLevel = newVal; }
	virtual bool GetDataNextLevel_Implementation() const override { return bDataNextLevel; }
	virtual void SetCombatModeNextLevel_Implementation(bool newVal) override { bCombatNextLevel = newVal; }
	virtual bool GetCombatModeNextLevel_Implementation() const override { return bCombatNextLevel; }

	/*
		IManageSingleton インタフェース
	*/
	virtual UAudioManager* GetAudioManager_Implementation() override;
	virtual UDataStorage* GetDataStorage_Implementation() override;
	virtual UQuestManager* GetQuestManager_Implementation() override;
	
	/*
		バフリスト
	*/
	UFUNCTION()
	TArray<class UBuff*> GetBuffListInstance() const;
	UFUNCTION()
	void SetBuffListInstance(TArray<class UBuff*> buffList);
	
private:

	/*
		レブル間のデータ
	*/
	// ** プレイヤーのバフリスト ** //
	UPROPERTY(VisibleAnywhere)
	TArray<class UBuff*> playerBuffList;
	
	// ** メインメニュー ** //
	UPROPERTY(VisibleAnywhere)
	bool bNewGameStarted = false;
	UPROPERTY(VisibleAnywhere)
	bool bMainMenuContinue = false;
	UPROPERTY(VisibleAnywhere)
	bool bDataNextLevel = false;
	UPROPERTY(VisibleAnywhere)
	bool bCombatNextLevel = false;
	
	/*
		ゲームのデータ
	*/
	// ** プレイヤーのセーブデータ ** //
	UPROPERTY(VisibleAnywhere)
	class UMainSaveGame* mainSaveGame = nullptr;
	UPROPERTY(VisibleAnywhere)
	FString saveSlotName = "DefaultSaveSlot";

	/*
		シングルトン
	*/
	UPROPERTY(EditAnywhere, Category = "Managers")
	TSubclassOf<UAudioManager> audioManagerClass = nullptr;
	UPROPERTY(EditAnywhere, Category = "Managers")
	TSubclassOf<UDataStorage> dataStorageClass = nullptr;
	UPROPERTY(EditAnywhere, Category = "Managers")
	TSubclassOf<UQuestManager> questManagerClass = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Managers")
	UAudioManager* AudioManagerInstance = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Managers")
	UDataStorage* DataStorageInstance = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Managers")
	UQuestManager* QuestManagerInstance = nullptr;
};
