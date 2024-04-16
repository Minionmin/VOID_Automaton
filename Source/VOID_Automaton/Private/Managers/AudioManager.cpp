// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AudioManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UAudioManager::UAudioManager()
{
}

UAudioComponent* UAudioManager::PlayBGM(USoundBase* soundBase, UObject* worldContextObject)
{
	currentBGM = UGameplayStatics::SpawnSound2D(worldContextObject, soundBase, masterVolume * bgmVolume);
	return currentBGM;
}

UAudioComponent* UAudioManager::PlaySFX(USoundBase* soundBase, UObject* worldContextObject)
{
	currentSFX = UGameplayStatics::SpawnSound2D(worldContextObject, soundBase, masterVolume * sfxVolume);
	return currentSFX;
}


float UAudioManager::GetMasterVolume() const
{
	return masterVolume;
}

float UAudioManager::GetBGMVolume() const
{
	return bgmVolume;
}

float UAudioManager::GetSFXVolume() const
{
	return sfxVolume;
}

void UAudioManager::SetMasterVolume(float volume)
{
	masterVolume = volume;

	if (currentBGM != nullptr)
	{
		currentBGM->SetVolumeMultiplier(masterVolume * bgmVolume);
	}
	if (currentSFX != nullptr)
	{
		currentSFX->SetVolumeMultiplier(masterVolume * sfxVolume);
	}
}

void UAudioManager::SetBGMVolume(float volume)
{
	bgmVolume = volume;
	if (currentBGM != nullptr)
	{
		currentBGM->SetVolumeMultiplier(masterVolume * bgmVolume);
	}
}

void UAudioManager::SetSFXVolume(float volume)
{
	sfxVolume = volume;
	if (currentSFX != nullptr)
	{
		currentSFX->SetVolumeMultiplier(masterVolume * sfxVolume);
	}
}