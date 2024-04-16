// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBaseWidget.generated.h"

class IHasSaveGame;
class UButton;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMenuBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	UFUNCTION()
	virtual void OnOptionButtonClicked();
	UFUNCTION()
	virtual void OnQuitButtonClicked();


	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* optionButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* quitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> optionMenuWidgetClass;
	
	IHasSaveGame* hasSaveGame = nullptr;
};
