// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBaseWidget.h"
#include "PlayerInfoUI.generated.h"

class UImage;
class USizeBox;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UPlayerInfoUI : public UCharacterBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void SetHealthBarSize(float maxHealth);
	UFUNCTION()
	void SetStaminaBarSize(float maxStamina);
	UFUNCTION()
	virtual void SetHealthBarPercentage(float value) override;
	UFUNCTION()
	void SetStaminaBarPercentage(float value);
	UFUNCTION()
	void SetTweenStaminaBarPercentage(float value);
	UFUNCTION()
	void SetAmmoLeft(int32 ammoLeft);
	UFUNCTION()
	void UpdateBuffImages();
	UFUNCTION()
	void UpdatePotionUIs(const TArray<UTexture2D*>& potionTextures, const TArray<int>& potionCapacityArray);
	UFUNCTION()
	void UpdatePotionCapacity(const TArray<int>& potionCapacityArray);
	
protected:

	UFUNCTION()
	virtual void TweenBars(float InDeltaTime) override;
	UFUNCTION()
	bool IsTweenStaminaBarMatched() const;

protected:

	// ** 体力・スタミナバーの背景イメージボックス ** //
	UPROPERTY(meta = (BindWidget))
	USizeBox* healthBackgroundBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* staminaBackgroundBox;

	// ** 体力・スタミナバー ** //
	UPROPERTY(meta = (BindWidget))
	USizeBox* healthSizeBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* staminaSizeBox;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* staminaProgressBar;

	// ** Tween 体力・スタミナバー ** //
	UPROPERTY(meta = (BindWidget))
	USizeBox* tweenHealthSizeBox;
	UPROPERTY(meta = (BindWidget))
	USizeBox* tweenStaminaSizeBox;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* tweenStaminaProgressBar;

	// ** 弾数テキスト ** //
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ammoText;

	// ** バフアイコン ** //
	UPROPERTY(meta = (BindWidget))
	UImage* buffIcon1;
	UPROPERTY(meta = (BindWidget))
	UImage* buffIcon2;
	UPROPERTY(meta = (BindWidget))
	UImage* buffIcon3;
	UPROPERTY(meta = (BindWidget))
	UImage* buffIcon4;

	// ** ポーションアイコン ** //
	UPROPERTY(meta = (BindWidget))
	UImage* potionIcon1;
	UPROPERTY(meta = (BindWidget))
	UImage* potionIcon2;
	UPROPERTY(meta = (BindWidget))
	UImage* potionIcon3;
	UPROPERTY(meta = (BindWidget))
	UImage* potionIcon4;

	// ** ポーション数量の背景イメージ ** //
	UPROPERTY(meta = (BindWidget))
	UImage* potionCapacityImage1;
	UPROPERTY(meta = (BindWidget))
	UImage* potionCapacityImage2;
	UPROPERTY(meta = (BindWidget))
	UImage* potionCapacityImage3;
	UPROPERTY(meta = (BindWidget))
	UImage* potionCapacityImage4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* potionCapacityTexture;
	
	// ** ポーション数量テキスト ** //
	UPROPERTY(meta = (BindWidget))
	UTextBlock* potionCapacityText1;
	UPROPERTY(meta = (BindWidget))
    UTextBlock* potionCapacityText2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* potionCapacityText3;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* potionCapacityText4;
	
	// ** 値に対するバーの最小・最大幅 ** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float widthPerMaxHealth = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float widthPerMaxStamina = 6.f;

	// ** バーのTween速度 ** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float staminaBarInterpSpeed = 2.5f;

	// ** 透明テクスチャー ** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* transparentTexture;

	// ** GameInstance ** //
	UPROPERTY(VisibleAnywhere)
	class UMainGameInstance* gameInstance;
};
