// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageActor/DamageActorBase.h"
#include "PostEffectDamageActor.generated.h"

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API APostEffectDamageActor : public ADamageActorBase
{
	GENERATED_BODY()
public:	

	APostEffectDamageActor();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> cameraShakeClass; // shake upon spawn
};
