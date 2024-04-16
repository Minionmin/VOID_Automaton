// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionBaseWidget.generated.h"

class IHasSaveGame;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UOptionBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	
	virtual void ApplySetting();
	
	virtual void RevertSetting()
	{
		UE_LOG(LogTemp, Warning, TEXT("RevertSetting Base"));
	};

protected:
	
	virtual void InitializeSetting()
	{
		UE_LOG(LogTemp, Warning, TEXT("InitializeSetting Base"));
	};

	UPROPERTY(VisibleAnywhere)
	bool bIsNewGame = false;
	IHasSaveGame* hasSaveGame = nullptr;
};
