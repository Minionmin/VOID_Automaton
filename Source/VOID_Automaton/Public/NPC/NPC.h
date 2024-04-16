// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HasInstructionWidget.h"
#include "Interfaces/Interactable.h"
#include "NPC.generated.h"

class USphereComponent;

UCLASS()
class VOID_AUTOMATON_API ANPC : public ACharacter, public IInteractable, public IHasInstructionWidget
{
	GENERATED_BODY()

public:

	ANPC();
	virtual void Tick(float DeltaTime) override;

	// Interface
	virtual void Interact_Implementation(AVOIDCharacter* character) override;
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
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* areaSphere;

	/*
		Configures
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
	FString itemName = "Default"; // the name which appears on the widget
	
	/*
		UI
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* instructionWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> npcWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* npcWidgetInstance;
	
	/*
		General
	*/
	UPROPERTY(VisibleAnywhere, Category = "General")
	AVOIDCharacter* VOIDcharacter; // Pointer to player

	/*
		Animmontages
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animmontages")
	UAnimMontage* interactMontage;

public:
	FORCEINLINE UWidgetComponent* GetPickUpWidget() const { return instructionWidget; }
};
