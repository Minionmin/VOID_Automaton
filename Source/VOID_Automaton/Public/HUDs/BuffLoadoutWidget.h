// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffLoadoutWidget.generated.h"

class UButton;
class UListView;
class UDataTable;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UBuffLoadoutWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

	void InitializeBuffListView();
	
	UFUNCTION()
	void RemoveAllRegisteredBuff();
	void CloseBuffWidget();
	UFUNCTION()
	void OnBuffButtonClicked();
	UFUNCTION()
	void OnAddBuffButtonClicked();
	UFUNCTION()
	void OnConfirmButtonClicked();
	UFUNCTION()
	void OnCloseButtonClicked();

private:

	UFUNCTION()
	void SetButtonStyle(UButton* button, UTexture2D* buttonTexture);
	
private:

	// バッフボタン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* equippedBuffButton1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* equippedBuffButton2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* equippedBuffButton3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* equippedBuffButton4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* addBuffButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* confirmButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* closeButton;
	
	// バッフリスト
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UListView* buffListView;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	// UDataTable* buffListDataTable;
	
	// バッフ詳細
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* buffImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* buffDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* buffEffectDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* buffWeightText;

	// バフの重み計算
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 currentBuffWeight = 0; // 現在のバフの重量
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 maxBuffCapacity = 0; // バフの重量の最大値（プレイヤーのBuffComponentから取得）
	

	// 選択されているバフのクラス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UBuff> selectedBuffClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName selectedBuffID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText selectedBuffName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float selectedBuffStrength = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float selectedBuffWeight = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTexture2D* selectedBuffTexture = nullptr;

	// 登録されたバフのリスト
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class UBuff*> registeredBuffs;

	// プレイヤーとプレイヤーのバフコンポネントのレファレンス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AVOIDCharacter* playerCharacter = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBuffComponent* playerBuffComponent = nullptr;

	// バフが外される時のテクスチャー
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* transparentTexture = nullptr;
	
public:
	
	UFUNCTION()
	void SetSelectedBuffClass(TSubclassOf<UBuff> newBuffClass);
	UFUNCTION()
	void SetBuffImage(UTexture2D* newBuffImage);
	UFUNCTION()
	void SetBuffDescription(FText newBuffDesc);
	UFUNCTION()
	void SetBuffEffectDescription(FText newBuffEffectDesc);
	UFUNCTION()
	int32 GetCurrentBuffWeight() const { return currentBuffWeight; }
	UFUNCTION()
	void SetBuffWeightText(int32 newBuffWeight);
	UFUNCTION()
	void SetMaxBuffWeightText(int32 newMaxWeight);

	// 現在選択されているバフのデータを設定する
	UFUNCTION()
	void SetSelectedBuffID(FName newBuffID);
	UFUNCTION()
	void SetSelectedBuffName(FText newBuffName);
	UFUNCTION()
	void SetSelectedBuffStrength(float newBuffStrength);
	UFUNCTION()
	void SetSelectedBuffWeight(float newBuffWeight);
	UFUNCTION()
	void SetSelectedBuffTexture(UTexture2D* newBuffTexture);
};
