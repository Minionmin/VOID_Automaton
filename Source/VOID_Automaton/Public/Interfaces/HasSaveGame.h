// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Structures/PlayerSaveStruct.h"
#include "HasSaveGame.generated.h"

struct FPlayerSaveStruct;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHasSaveGame : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOID_AUTOMATON_API IHasSaveGame
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadGameData(bool bAsync = false);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveGameData(bool bAsync = false);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class UMainSaveGame* GetSaveGame();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SavePlayerStruct(FPlayerSaveStruct playerSaveData, bool bAsync = false);
	// メインメニューでNewGameボタンが押されたかどうか
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetNewGameStarted(bool newVal);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetNewGameStarted() const;
	// メインメニューで継続ボタンが押されたかどうか
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMainMenuContinue(bool newVal);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetMainMenuContinue() const;
	// レベル間でデータを渡すべきかどうか
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDataNextLevel(bool newVal);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetDataNextLevel() const;
	// レベル間で移動する時、戦闘モードに入るかどうか
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatModeNextLevel(bool newVal);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetCombatModeNextLevel() const;

};
