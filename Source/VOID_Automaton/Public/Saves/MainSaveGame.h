// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structures/PlayerSaveStruct.h"
#include "MainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UFUNCTION()
	FPlayerSaveStruct GetPlayerSaveData() const { return playerSaveData; }
	UFUNCTION()
	void SetPlayerSaveData(FPlayerSaveStruct newPlayerSaveData);
	
public:

	// プレイヤーのセーブデータ
	UPROPERTY()
	FPlayerSaveStruct playerSaveData;
};

// Unreal Engineのパイプライン
// Start Engine(Editor) -> (UEditorEngine)Init -> (UEngine)Start -> Uses presses "Play In Editor" button
// -> Create UGameInstance -> (UGameInstance)InitializePIE -> (UGameInstance)Init -> ...

// Start Engine(Standalone) -> (UEditorEngine)Init
// -> Create UGameInstance -> (UGameInstance)Initialize Standalone -> (UGameInstance)Init -> ...

// 上記を参考に、セーブデータの最適な保存場所はUGameInstanceクラスであることが分かった