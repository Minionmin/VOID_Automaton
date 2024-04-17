// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buff.generated.h"

class UAttributeComponent;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class VOID_AUTOMATON_API UBuff : public UObject
{
	GENERATED_BODY()

public:
	
	UBuff();

	UFUNCTION(BlueprintCallable)
	virtual void InitializeBuff(int newID, FString newName, float newStrength);
	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffect(AActor* target);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveEffect(AActor* target);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	int buffID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	FString buffName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float buffStrength = 0.f;

public:

	// ゲッター
	UFUNCTION(BlueprintCallable)
	FString GetBuffName() const { return buffName; }
	UFUNCTION(BlueprintCallable)
	int GetBuffID() const { return buffID; }
	
	// セッター
	UFUNCTION(BlueprintCallable)
	void SetBuffID(int newID);
	UFUNCTION(BlueprintCallable)
	void SetBuffName(FString newName);
	UFUNCTION(BlueprintCallable)
	void SetBuffStrength(float newStrength);
};
