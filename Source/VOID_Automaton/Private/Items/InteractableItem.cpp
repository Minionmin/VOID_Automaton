// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractableItem.h"
#include "VOIDCharacter.h"

AInteractableItem::AInteractableItem()
{
	// We're not using Tick event
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableItem::Interact_Implementation(AVOIDCharacter* character)
{
	
}