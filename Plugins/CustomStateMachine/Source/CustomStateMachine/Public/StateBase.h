// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CUSTOMSTATEMACHINE_API UStateBase : public UObject
{
	GENERATED_BODY()

public:
	
	virtual void OnEnterState(AActor* stateOwner);
	virtual void TickState(float deltaTime);
	virtual void OnExitState();
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanTickState = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanRepeat = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName stateDisplayName; // For debugging purpose

protected:

	// Is this the first time entering the state
	bool bFirstEnter = true;
};
