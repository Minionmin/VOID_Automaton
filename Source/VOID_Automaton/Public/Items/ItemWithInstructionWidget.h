// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "ItemWithInstructionWidget.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API AItemWithInstructionWidget : public AItem, public IHasInstructionWidget
{
	GENERATED_BODY()

public:

	AItemWithInstructionWidget();
	virtual void Tick(float DeltaTime) override;

	// Interface
	virtual UWidgetComponent* GetInstructionWidget_Implementation() override;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, 
		UPrimitiveComponent* otherComp, int32 otherBodyIndex);

protected:

	/*
		Configures
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
	FString itemName = "Default"; // the name which appears on the widget
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* areaSphere;

	/*
		UI
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI | Widget")
	UWidgetComponent* instructionWidget;
};
