// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReloadWidget.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UReloadWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateReloadProgress(float percentVal);
	UFUNCTION()
	void Show();
	UFUNCTION()
	void Hide();
	
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* reloadIconParentMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* reloadIconDynamicInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget = "ReloadIconImage", AllowPrivateAccess = "true"))
	class UImage* reloadIconImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget = "AmmoIconImage", AllowPrivateAccess = "true"))
	class UImage* ammoIconImage;
};
