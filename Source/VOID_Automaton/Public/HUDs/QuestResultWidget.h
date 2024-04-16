// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structures/ItemListStruct.h"
#include "QuestResultWidget.generated.h"

class UTextBlock;
class UButton;
class UListView;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UQuestResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetQuestName(FText name);
	UFUNCTION(BlueprintCallable)
	void SetQuestTimer(FText time);
	UFUNCTION(BlueprintCallable)
	void SetHeadshotCount(int count);
	UFUNCTION(BlueprintCallable)
	void SetBodyshotCount(int count);
	UFUNCTION(BlueprintCallable)
	void SetDamageDealt(int damage);
	UFUNCTION(BlueprintCallable)
	void SetDamageTaken(int damage);
	UFUNCTION(BlueprintCallable)
	void SetDashAmount(int amount);
	UFUNCTION(BlueprintCallable)
	void SetAccuracyText(float overallAccuracy);
	void SetItemDropList(const TArray<TPair<FItemListStruct*, int>>& itemPairArray);

	UFUNCTION()
	void OnBackToShipButtonClicked();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* questNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* questTimerText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* headshotText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* bodyshotText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* damageDealtText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* damageTakenText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* dashAmountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* accuracyText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UListView* itemDropListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* backToShipButton;
	
};
