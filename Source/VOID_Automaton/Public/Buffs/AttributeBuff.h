// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buffs/Buff.h"
#include "AttributeBuff.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UAttributeBuff : public UBuff
{
	GENERATED_BODY()

public:

	UAttributeBuff();

	virtual void ApplyEffect(AActor* target) override;
	virtual void RemoveEffect(AActor* target) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Buff")
	UAttributeComponent* attributeComponent;
};
