// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Potion.h"


UPotion::UPotion()
{
}

void UPotion::Interact_Implementation(AVOIDCharacter* character)
{
	UE_LOG(LogTemp, Warning, TEXT("Potion Interacted"));
}
