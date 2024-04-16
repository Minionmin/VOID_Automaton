// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interactable.h"
#include "UObject/NoExportTypes.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VOID_AUTOMATON_API UPotion : public UObject, public IInteractable
{
	GENERATED_BODY()

public:

	UPotion();

	virtual void Interact_Implementation(class AVOIDCharacter* character) override;

	UFUNCTION(BlueprintCallable, Category = "Potion")
	FString GetPotionName() const { return potionName; }
	UFUNCTION(BlueprintCallable, Category = "Potion")
	float GetPotionStrength() const { return potionStrength; }
	UFUNCTION(BlueprintCallable, Category = "Potion")
	UTexture2D* GetPotionTexture2D() const { return potionTexture2D; }
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	FString potionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	float potionStrength = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion")
	UParticleSystem* potionParticle = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* potionTexture2D = nullptr;
};
