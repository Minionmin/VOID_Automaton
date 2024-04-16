// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemWithInstructionWidget.h"

#include "VOIDCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AItemWithInstructionWidget::AItemWithInstructionWidget()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Area for Showing/Hiding UI
	areaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	areaSphere->SetupAttachment(itemMesh);
	areaSphere->SetSphereRadius(400.f);
	areaSphere->CanCharacterStepUpOn = ECB_No;
	areaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	areaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Overlap event only for WorldDynamic and Pawn
	areaSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	areaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// "Press E to interact" kind of UI
	instructionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InstructionWidget"));
	instructionWidget->SetupAttachment(itemMesh);
}

void AItemWithInstructionWidget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UWidgetComponent* AItemWithInstructionWidget::GetInstructionWidget_Implementation()
{
	return instructionWidget;
}

void AItemWithInstructionWidget::BeginPlay()
{
	Super::BeginPlay();
	
	// Hide widget first
	instructionWidget->SetVisibility(false);

	// Setup overlap for AreaSphere
	areaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemWithInstructionWidget::OnSphereOverlap);
	areaSphere->OnComponentEndOverlap.AddDynamic(this, &AItemWithInstructionWidget::OnSphereEndOverlap);

}

void AItemWithInstructionWidget::OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
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

void AItemWithInstructionWidget::OnSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
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