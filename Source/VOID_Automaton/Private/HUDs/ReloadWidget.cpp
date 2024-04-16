// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/ReloadWidget.h"

#include "Components/Image.h"
#include "VOID_Automaton/DebugMacros.h"

void UReloadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	reloadIconDynamicInstance = UMaterialInstanceDynamic::Create(reloadIconParentMaterial, this);
	if(reloadIconDynamicInstance)
	{
		// Create brush from dynamic material
		reloadIconImage->SetBrushFromMaterial(reloadIconDynamicInstance);
		
		// Then hide other components (include the progress bar itself)
		Hide();
	}
}

void UReloadWidget::UpdateReloadProgress(float percentVal)
{
	reloadIconDynamicInstance->SetScalarParameterValue(TEXT("Percent"), FMath::Clamp(percentVal, 0.f, 1.f));
}

void UReloadWidget::Show()
{
	UpdateReloadProgress(0.0f);
	reloadIconImage->SetVisibility(ESlateVisibility::Visible);
	ammoIconImage->SetVisibility(ESlateVisibility::Visible);
}

void UReloadWidget::Hide()
{
	UpdateReloadProgress(0.0f);
	reloadIconImage->SetVisibility(ESlateVisibility::Hidden);
	ammoIconImage->SetVisibility(ESlateVisibility::Hidden);
}
