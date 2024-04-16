// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structures/PlayerInventoryStruct.h"
#include "PlayerInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_AUTOMATON_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
								   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void AddItem(int itemIDToCheck, int amountToAdd);
	UFUNCTION()
	void RemoveItem(const FPlayerInventoryStruct& itemToRemove);
	UFUNCTION()
	bool HasItem(int itemIDToCheck);
	UFUNCTION()
	int GetItemAmount(int itemIDToCheck);
	UFUNCTION()
	TArray<int> GetItemIDList() const;
	
protected:

	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, int> itemInventory; // <ItemID, Amount>
};
