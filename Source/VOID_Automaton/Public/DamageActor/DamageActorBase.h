// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HasGameplayTag.h"
#include "DamageActorBase.generated.h"

class UBoxComponent;

UCLASS()
class VOID_AUTOMATON_API ADamageActorBase : public AActor, public IHasGameplayTag
{
	GENERATED_BODY()
	
public:	

	ADamageActorBase();
	virtual void Tick(float DeltaTime) override;

	virtual FGameplayTagContainer GetGameplayTagContainer_Implementation() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, 
		int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, 
		UPrimitiveComponent* otherComp, int32 otherBodyIndex);
	
protected:

	virtual void BeginPlay() override;

protected:
	
	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* boxCollision;

	/*
		Tag
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer actorTagsContainer;
	
	/*
		Stat 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lifeTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float baseDamage = 10.f;
};
