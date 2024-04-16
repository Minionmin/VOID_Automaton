// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuffListChanged);

class UBuff;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOID_AUTOMATON_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBuffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
		イベント
	*/
	FOnBuffListChanged OnBuffListChanged;
	
	UFUNCTION()
	void AddBuff(UBuff* buff);
	UFUNCTION()
	void RemoveBuff(UBuff* buff);
	UFUNCTION()
	void ApplyAllBuffEffect();
	UFUNCTION()
	void RemoveAllBuffs();
	UFUNCTION()
	void ReloadBuffList();
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TArray<UBuff*> activeBuffs;
	UPROPERTY(VisibleAnywhere)
	AActor* buffOwner = nullptr;
	UPROPERTY(EditAnywhere, Category = "Buff", meta = (AllowPrivateAccess = "true"))
	int32 maxBuffWeight = 0; // バフの重量の最大値

public:

	UFUNCTION(BlueprintCallable)
	TArray<UBuff*> GetActiveBuffs() { return activeBuffs; }
	UFUNCTION(BlueprintCallable)
	void GetActiveBuffIDs(TArray<FName>& buffSaveDataArray) const;
	UFUNCTION(BlueprintCallable)
	void SetActiveBuffs(const TArray<UBuff*>& buffList);
	UFUNCTION(BlueprintCallable)
	void LoadActiveBuffs();
	UFUNCTION(BlueprintCallable)
	int32 GetBuffMaxCapacity() const { return maxBuffWeight; }
	UFUNCTION(BlueprintCallable)
	void SetMaxBuffWeight(int32 newMaxWeight) { maxBuffWeight = newMaxWeight; }
};
