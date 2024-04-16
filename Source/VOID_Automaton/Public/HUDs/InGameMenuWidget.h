// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBaseWidget.h"
#include "InGameMenuWidget.generated.h"

class AVOIDCharacter;
/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UInGameMenuWidget : public UMenuBaseWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void LoadPlayerSaveData();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* saveButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* loadButton;

	UPROPERTY(VisibleAnywhere)
	AVOIDCharacter* VOIDCharacter = nullptr;
};
