// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableItem.h"
#include "Items/Item.h"
#include "Interfaces/Interactable.h"
#include "PickableItem.generated.h"

class AVOIDCharacter;
class UCurveFloat;

UENUM(BlueprintType, meta = (ScriptName = "ItemPickRarity"))
enum class EItemPickRarity : uint8
{
	EIPR_NoRarity UMETA(DisplayName = "No Rarity"),
	EIPR_Common UMETA(DisplayName = "Common"),
	EIPR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIPR_Rare UMETA(DisplayName = "Rare"),
	EIPR_SuperRare UMETA(DisplayName = "Super Rare"),
	EIPR_Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Falling UMETA(DisplayName = "Falling"),
	EIS_PickInterping UMETA(DisplayName = "PickInterping"),
	EIS_PickedUp UMETA(DisplayName = "Pickedup")
};

UCLASS()
class VOID_AUTOMATON_API APickableItem : public AInteractableItem
{
	GENERATED_BODY()

public:

	APickableItem();
	virtual void Tick(float DeltaTime) override;
	
	
	virtual void Interact_Implementation(AVOIDCharacter* character) override;

protected:

	virtual void BeginPlay() override;
	
	/*
		Widget (UI)
	*/
	// Set the activeStars array of bools based on rarity
	void SetActiveStars();

	/*
		ItemState
	*/
	void SetItemState(EItemState state);

	/*
		Animation
	*/
	void StartItemInterping();
	void FinishItemInterping();
	void InterpItem(float deltaTime);
	
protected:

	/*
		General
	*/
	UPROPERTY(VisibleAnywhere, Category = "General | Interping")
	FVector itemInterpStartLocation = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, Category = "General | Interping")
	float itemInterpInitialYawOffset = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "General | Interping")
	UCurveFloat* itemZCurve; // The curve asset to use for the item's Z location when interping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General | Interping")
	float ZCurveTime = 0.7f;
	UPROPERTY(EditDefaultsOnly, Category = "General | Interping")
	UCurveFloat* itemScaleCurve; // The curve asset to use for the item's scale when interping
	
	FTimerHandle itemInterpTimer; // Play on start interping
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General | State")
	EItemState itemState = EItemState::EIS_Falling; // determines the current state of item
	UPROPERTY(VisibleAnywhere, Category = "General | State")
	bool bIsInterping = false;
	
	UPROPERTY(VisibleAnywhere, Category = "General")
	AVOIDCharacter* VOIDcharacter; // Pointer to player
	UPROPERTY(VisibleAnywhere, Category = "General")
	FVector itemInitialScale = FVector(1.f); // Item's default scale
	
	/*
		Configures
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General | Rarity")
	EItemPickRarity itemRarity = EItemPickRarity::EIPR_NoRarity; // determines number of star on the widget

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "General | Rarity")
	TArray<bool> activeStars;

public:
	FORCEINLINE EItemPickRarity GetItemRarity() const { return itemRarity; }
	FORCEINLINE EItemState GetItemState() const { return itemState; }
};
