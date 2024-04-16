// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateBase.h"
#include "StateManagerComponent.generated.h"

USTRUCT()
struct FTimerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FTimerHandle timerHandle;
	UPROPERTY()
	FString name;
	UPROPERTY()
	FString owner;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUSTOMSTATEMACHINE_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UStateManagerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void SwitchState(UStateBase* newState);
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void SwitchStateByKey(FString stateKey);
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void InitStateManager();
	
protected:
	
	virtual void BeginPlay() override;

private:

	void InitializeState();
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine")
	FString initialState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine")
	TMap<FString, TSubclassOf<UStateBase>> availableStates; // The state must be in this list to use
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine Debug")
	bool bDebug = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine Debug", meta = (EditCondition = "bDebug", EditConditionHides = "bDebug"))
	bool bShowStateProgress = false;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UStateBase*> stateHistory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Machine Debug", meta = (ClampMin = "1", ClampMax = "10", UIMin = "1", UIMax = "10"))
	int32 stateHistoryLength = 1;

	UPROPERTY(BlueprintReadOnly)
	UStateBase* currentState = nullptr;
	UPROPERTY(VisibleAnywhere)
	TMap<FString, UStateBase*> stateMap;

private:
	UPROPERTY(EditDefaultsOnly, Category = "State Machine")
	bool bCanTickState = true;
};
