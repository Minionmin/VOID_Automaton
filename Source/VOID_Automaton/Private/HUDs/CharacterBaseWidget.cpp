// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/CharacterBaseWidget.h"

#include "MyCustomUnrealLibrary.h"
#include "Components/ProgressBar.h"

void UCharacterBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCharacterBaseWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 体力・スタミナの値が変更する時に、Tween演出
	TweenBars(InDeltaTime);
}

void UCharacterBaseWidget::SetHealthBarPercentage(float value)
{
	healthProgressBar->SetPercent(value);
}

void UCharacterBaseWidget::TweenBars(float InDeltaTime)
{
	// Tweenの体力バーが現在の体力バーと一致していない場合、Tweenの体力バーを更新
	if(!IsTweenHealthBarMatched())
	{
		// Tween体力バーを本の体力バーに合わせる
		UUIUtilities::InterpProgressBar(tweenHealthProgressBar, healthProgressBar->GetPercent(), InDeltaTime, healthBarInterpSpeed);	
	}
}

bool UCharacterBaseWidget::IsTweenHealthBarMatched() const
{
	return tweenHealthProgressBar->GetPercent() == healthProgressBar->GetPercent();
}
