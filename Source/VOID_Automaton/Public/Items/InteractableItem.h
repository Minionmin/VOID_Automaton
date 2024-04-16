// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWithInstructionWidget.h"
#include "Interfaces/Interactable.h"
#include "InteractableItem.generated.h"

class AVOIDCharacter;

UCLASS()
class VOID_AUTOMATON_API AInteractableItem : public AItemWithInstructionWidget, public IInteractable
{
	GENERATED_BODY()
	
public:

	AInteractableItem();
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation(AVOIDCharacter* character) override;

protected:

	virtual void BeginPlay() override;
	
protected:

	/*
		Configures
	*/

};
