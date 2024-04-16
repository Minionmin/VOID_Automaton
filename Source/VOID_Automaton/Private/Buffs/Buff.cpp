// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/Buff.h"

UBuff::UBuff()
{
}

void UBuff::ApplyEffect(AActor* target)
{
}

void UBuff::RemoveEffect(AActor* target)
{
}

void UBuff::InitializeBuff(FName newID, FString newName, float newStrength)
{
	buffID = newID;
	buffName = newName;
	buffStrength = newStrength;
}

void UBuff::SetBuffID(FName newID)
{
	buffID = newID;
}

void UBuff::SetBuffName(FString newName)
{
	buffName = newName;
}

void UBuff::SetBuffStrength(float newStrength)
{
	buffStrength = newStrength;
}
