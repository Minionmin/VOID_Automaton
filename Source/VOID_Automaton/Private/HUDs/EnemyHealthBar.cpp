// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/EnemyHealthBar.h"
#include "Components/TextBlock.h"


void UEnemyHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UEnemyHealthBar::SetHealthBarPercentage(float value)
{
	Super::SetHealthBarPercentage(value);
}

void UEnemyHealthBar::SetEnemyName(FString text)
{
	enemyNameText->SetText(FText::FromString(text));
}

void UEnemyHealthBar::TweenBars(float InDeltaTime)
{
	Super::TweenBars(InDeltaTime);
}
