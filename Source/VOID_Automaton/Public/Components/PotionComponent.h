// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PotionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPotionUsed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPotionCapacityChanged, const TArray<int>&, potionCapacityArray);

class UPotion;
class UTexture2D;
class AVOIDCharacter;

USTRUCT(BlueprintType)
struct FPotionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPotion> potionType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int quantity = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOID_AUTOMATON_API UPotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPotionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
		イベント
	*/
	FOnPotionUsed OnPotionUsed;
	FOnPotionCapacityChanged OnPotionCapacityChanged;
	
	UFUNCTION()
	void AddPotion(TSubclassOf<UPotion> potionType, int quantity);
	UFUNCTION()
	void UsePotionSlot(int potionSlot, AVOIDCharacter* usingCharacter);
	void UsePotion(FPotionInfo& potionInfo, AVOIDCharacter* usingCharacter);
	
	UFUNCTION()
	TArray<UTexture2D*> GetPotionTextures();
	UFUNCTION()
	TArray<int> GetPotionCapacityArray();
	
protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	TArray<FPotionInfo> potionInventory;
};
