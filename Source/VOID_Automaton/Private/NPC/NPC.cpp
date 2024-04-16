// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NPC.h"

#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "VOID_Automaton/DebugMacros.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set capsule collision to block visibility for interaction
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	// Area for Showing/Hiding UI
	areaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	areaSphere->SetupAttachment(GetRootComponent());
	areaSphere->SetSphereRadius(400.f);
	areaSphere->CanCharacterStepUpOn = ECB_No;
	areaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	areaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Overlap event only for WorldDynamic and Pawn
	areaSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	areaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// "Press E to interact" kind of UI
	instructionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InstructionWidget"));
	instructionWidget->SetupAttachment(GetRootComponent());
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Hide widget first
	instructionWidget->SetVisibility(false);

	// Setup overlap for AreaSphere
	areaSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnSphereOverlap);
	areaSphere->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnSphereEndOverlap);
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC::OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
                           int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Update player's overlapped item count
	if(otherActor)
	{
		if(AVOIDCharacter* VOIDCharacter = Cast<AVOIDCharacter>(otherActor))
		{
			VOIDCharacter->UpdateOverlappedInteractCount(1);
		}
	}
}

void ANPC::OnSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	// Update player's overlapped item count
	if(otherActor)
	{
		if(AVOIDCharacter* VOIDCharacter = Cast<AVOIDCharacter>(otherActor))
		{
			VOIDCharacter->UpdateOverlappedInteractCount(-1);
		}
	}
}

void ANPC::Interact_Implementation(AVOIDCharacter* character)
{
	// Store player reference
	if(VOIDcharacter == nullptr)
	{
		VOIDcharacter = character;
	}

	// Play animation
	if(interactMontage)
	{
		PlayAnimMontage(interactMontage);
	}

	// Show widget
	if(npcWidgetClass)
	{
		npcWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), npcWidgetClass);
		npcWidgetInstance->AddToViewport();
		
		// Set focus to UI
		UUIUtilities::SetFocusUI(this);
	}
}

UWidgetComponent* ANPC::GetInstructionWidget_Implementation()
{
	return instructionWidget;
}
