// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterBaseWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UCharacterBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void SetHealthBarPercentage(float value);
	
protected:
	
	virtual void TweenBars(float InDeltaTime);
	UFUNCTION()
	bool IsTweenHealthBarMatched() const;
	
protected:

	// ** 体力・スタミナバー ** //
	UPROPERTY(meta = (BindWidget))
	UProgressBar* healthProgressBar;

	// ** Tween 体力・スタミナバー ** //
	UPROPERTY(meta = (BindWidget))
	UProgressBar* tweenHealthProgressBar;

	// ** バーのTween速度 ** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float healthBarInterpSpeed = 2.5f;
};
