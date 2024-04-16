// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBaseWidget.h"
#include "EnemyHealthBar.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UEnemyHealthBar : public UCharacterBaseWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	virtual void SetHealthBarPercentage(float value) override;
	UFUNCTION()
	void SetEnemyName(FString text);

protected:
	
	UFUNCTION()
	virtual void TweenBars(float InDeltaTime) override;
	
protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* enemyNameText;
};
