// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerInputManagerComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "VOIDCharacter.h"


UPlayerInputManagerComponent::UPlayerInputManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInputManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInputManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInputManagerComponent::Initialize(AVOIDCharacter* ownerCharacter, UInputComponent* playerInputComponent)
{
	// Shallow copy
	VOIDCharacter = ownerCharacter;
	
	if (APlayerController* PlayerController = Cast<APlayerController>(VOIDCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(VOIDContext, 0);
		}
	}

	// インプットを関数にバインドする
	SetupPlayerInputFunction(playerInputComponent);
}

void UPlayerInputManagerComponent::SetupPlayerInputFunction(UInputComponent* playerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent))
	{
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, VOIDCharacter, &AVOIDCharacter::Move);
		EnhancedInputComponent->BindAction(runAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::Run);
		EnhancedInputComponent->BindAction(runAction, ETriggerEvent::Completed, VOIDCharacter, &AVOIDCharacter::CancelRun);
		EnhancedInputComponent->BindAction(dashAction, ETriggerEvent::Triggered, VOIDCharacter, &AVOIDCharacter::StartDash);
		EnhancedInputComponent->BindAction(mouseLookAction, ETriggerEvent::Triggered, VOIDCharacter, &AVOIDCharacter::MouseLook);
		EnhancedInputComponent->BindAction(controllerLookAction, ETriggerEvent::Triggered, VOIDCharacter, &AVOIDCharacter::ControllerLook);
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::Jump);
		EnhancedInputComponent->BindAction(interactAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::Interact);
		EnhancedInputComponent->BindAction(fireAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::StartFire);
		EnhancedInputComponent->BindAction(fireAction, ETriggerEvent::Completed, VOIDCharacter, &AVOIDCharacter::StopFire);
		EnhancedInputComponent->BindAction(aimAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::Aim);
		EnhancedInputComponent->BindAction(aimAction, ETriggerEvent::Completed, VOIDCharacter, &AVOIDCharacter::StopAim);
		EnhancedInputComponent->BindAction(reloadAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::BeginReload);
		EnhancedInputComponent->BindAction(ESCAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::ESC);
		EnhancedInputComponent->BindAction(useConsumableAction, ETriggerEvent::Started, VOIDCharacter, &AVOIDCharacter::UsePotion);
	}
}

void UPlayerInputManagerComponent::SetBaseTurnRate(float newRate)
{
	baseTurnRate = FMath::Clamp(newRate, BASE_TURN_RATE_MIN, BASE_TURN_RATE_MAX);
}

void UPlayerInputManagerComponent::SetControllerHipTurnRate(float newRate)
{
	controllerHipTurnRate = FMath::Clamp(newRate, CONTROLLER_HIP_TURN_RATE_MIN, CONTROLLER_HIP_TURN_RATE_MAX);
}

void UPlayerInputManagerComponent::SetControllerAimingTurnRate(float newRate)
{
	controllerAimingTurnRate = FMath::Clamp(newRate, CONTROLLER_AIM_TURN_RATE_MIN, CONTROLLER_AIM_TURN_RATE_MAX);
}

void UPlayerInputManagerComponent::SetMouseHipTurnRate(float newRate)
{
	mouseHipTurnRate = FMath::Clamp(newRate, MOUSE_HIP_TURN_RATE_MIN, MOUSE_HIP_TURN_RATE_MAX);
}

void UPlayerInputManagerComponent::SetMouseAimingTurnRate(float newRate)
{
	mouseAimingTurnRate = FMath::Clamp(newRate, MOUSE_AIM_TURN_RATE_MIN, MOUSE_AIM_TURN_RATE_MAX); 
}