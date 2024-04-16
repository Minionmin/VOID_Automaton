// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageActor/PostEffectDamageActor.h"

#include "Kismet/GameplayStatics.h"

APostEffectDamageActor::APostEffectDamageActor()
{
}

void APostEffectDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APostEffectDamageActor::BeginPlay()
{
	Super::BeginPlay();

	// Append camera shake to player
	if(cameraShakeClass)
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(cameraShakeClass);	
	}
}
