// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/PlaySFXNotify.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/AudioManager.h"
#include "Saves/MainGameInstance.h"

void UPlaySFXNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(IManageSingleton* singletonManager = Cast<IManageSingleton>(GetWorld()->GetGameInstance()))
	{
		singletonManager->GetAudioManager_Implementation()->PlaySFX(soundBase, GetWorld());
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), soundBase, 1.0f);
	}
}
