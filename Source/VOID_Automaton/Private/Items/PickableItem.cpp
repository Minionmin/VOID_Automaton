// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickableItem.h"
#include "VOIDCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "VOID_Automaton/DebugMacros.h"

APickableItem::APickableItem()
{
	// We're using Tick event
	PrimaryActorTick.bCanEverTick = true;
}

void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle picking up animation
	InterpItem(DeltaTime);
}

void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	
	// Set rarity stars on Pickup widget
	SetActiveStars();
	SetItemState(EItemState::EIS_Falling);
}

void APickableItem::SetActiveStars()
{
	// Initializing star array
	// We won't use use the 0th element
	for(int8 i = 0; i <= 5; i++)
	{
		activeStars.Add(false);
	}

	// Set star visibility
	switch(itemRarity)
	{
	case EItemPickRarity::EIPR_Common:
		activeStars[1] = true;
		break;
	case EItemPickRarity::EIPR_Uncommon:
		activeStars[1] = true;
		activeStars[2] = true;
		break;
	case EItemPickRarity::EIPR_Rare:
		activeStars[1] = true;
		activeStars[2] = true;
		activeStars[3] = true;
		break;
	case EItemPickRarity::EIPR_SuperRare:
		activeStars[1] = true;
		activeStars[2] = true;
		activeStars[3] = true;
		activeStars[4] = true;
		break;
	case EItemPickRarity::EIPR_Legendary:
		activeStars[1] = true;
		activeStars[2] = true;
		activeStars[3] = true;
		activeStars[4] = true;
		activeStars[5] = true;
		break;
	default:
		break;
	}
}

void APickableItem::SetItemState(EItemState state)
{
	itemState = state;
	
	switch(state)
	{
	case EItemState::EIS_Falling:
		// Enable Physics, Query and Visibility
		itemMesh->SetSimulatePhysics(true);
		itemMesh->SetEnableGravity(true);
		itemMesh->SetVisibility(true);
		itemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		itemMesh ->SetCollisionResponseToAllChannels(ECR_Block);
		itemMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		itemMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		break;
	case EItemState::EIS_PickInterping:
		// Disable UI, Physics and Query
		// Enable Visibility
		instructionWidget->SetVisibility(false);
		itemMesh->SetSimulatePhysics(false);
		itemMesh->SetEnableGravity(false);
		itemMesh->SetVisibility(true);
		itemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		itemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		break;
	}
}

void APickableItem::StartItemInterping()
{
	bIsInterping = true;

	if(VOIDcharacter)
	{
		// Item's starting location on picking up
		itemInterpStartLocation = GetActorLocation();
		// Item's starting rotation on picking up (Yaw axis of Camera rotation - item rotation)
		itemInterpInitialYawOffset = (VOIDcharacter->GetCamera()->GetComponentRotation() - GetActorRotation()).Yaw;
		//Item's starting scale on picking up
		itemInitialScale = GetActorScale();
	}
	SetItemState(EItemState::EIS_PickInterping);

	// Start interping animation
	GetWorldTimerManager().SetTimer(itemInterpTimer, this, &APickableItem::FinishItemInterping, ZCurveTime);
}

void APickableItem::FinishItemInterping()
{
	bIsInterping = false;
	if(VOIDcharacter)
	{
		// Add item to player's inventory
		VOIDcharacter->GetPickUpItem(this);
		SetItemState(EItemState::EIS_Falling);
	}

	// Returning item to normal scale
	SetActorScale3D(itemInitialScale);
}

void APickableItem::InterpItem(float deltaTime)
{
	if(!bIsInterping) return;

	if(VOIDcharacter && itemZCurve && itemScaleCurve)
	{
		// ** Updating Location ** //
		// Elapsed time since we started itemInterpTimer
		const float elapsedTime = GetWorldTimerManager().GetTimerElapsed(itemInterpTimer);
		// Get curve value corresponding to elapsedTime
		const float curveValue = itemZCurve->GetFloatValue(elapsedTime);
		// Item position & Camera position
		FVector itemLocation = itemInterpStartLocation;
		const FVector cameraInterpLocation = VOIDcharacter->GetCameraInterpLocation();
		// Get vector between item Z axis and camera Z axis
		const FVector itemToCamera = FVector(0.f, 0.f, (cameraInterpLocation - itemLocation).Z);
		// Get Z delta size between item and camera (scale factor to curve value)
		const float deltaZ = itemToCamera.Size();

		const FVector currentLocation = GetActorLocation();
		// Interpolate X and Y value
		const float interpXvalue = FMath::FInterpTo(currentLocation.X, cameraInterpLocation.X, deltaTime, 30.f);
		const float interpYvalue = FMath::FInterpTo(currentLocation.Y, cameraInterpLocation.Y, deltaTime, 30.f);
		// Set X and Y of ItemLocation to interped value
		itemLocation.X = interpXvalue;
		itemLocation.Y = interpYvalue;
		itemLocation.Z += curveValue * deltaZ;
		// Begin moving the item
		SetActorLocation(itemLocation, true, nullptr, ETeleportType::TeleportPhysics);

		// ** Updating Rotation ** //
		const FRotator cameraThisFrameRotation = VOIDcharacter->GetCamera()->GetComponentRotation();
		const FRotator itemRotation = FRotator(0.f, cameraThisFrameRotation.Yaw + itemInterpInitialYawOffset, 0.f);
		SetActorRotation(itemRotation, ETeleportType::TeleportPhysics);

		// ** Updating Scale ** //
		const float scaleCurveValue = itemScaleCurve->GetFloatValue(elapsedTime);
		SetActorScale3D(FVector(scaleCurveValue));
	}
}

void APickableItem::Interact_Implementation(AVOIDCharacter* character)
{
	// Store player reference
	if(VOIDcharacter == nullptr)
	{
		VOIDcharacter = character;
	}

	// Start picking up animation
	StartItemInterping();	
}
