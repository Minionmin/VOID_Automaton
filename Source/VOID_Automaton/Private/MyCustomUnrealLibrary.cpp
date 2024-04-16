// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomUnrealLibrary.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/ManageSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


#pragma region ActorUtilities
/* Calculates the rotation from the main actor towards the target actor.
 * @param bVerticalComponent If true, includes the vertical component in the calculated rotation.
 *                           If false, eliminates the vertical component from the resulting rotation.
 *                           Default is false.
 */
FRotator UActorUtilities::GetRotatorTowardActor(const FVector& thisLocation, const FVector& targetLocation, bool bVerticalComponent)
{
	FVector toTargetVector = targetLocation - thisLocation;

	// Should have vertical component or not
	if(!bVerticalComponent)
	{
		// Eliminate vertical component by setting Z to 0
		toTargetVector.Z = 0.f;
	}

	// Check if the vector is not zero to avoid division by zero
	if (!toTargetVector.IsNearlyZero())
	{
		// Normalize the vector to ensure it has unit length
		toTargetVector.Normalize();

		return toTargetVector.Rotation();
	}

	// return zero if the vector is nearly zero
	return FRotator::ZeroRotator;
}

FRotator UActorUtilities::GetRotatorTowardActor(const AActor* thisActor, const AActor* targetActor, bool bVerticalComponent)
{
	return GetRotatorTowardActor(thisActor->GetActorLocation()
		, targetActor->GetActorLocation(), bVerticalComponent);
}

/*
 * Make an actor face another actor smoothly
 */
void UActorUtilities::FaceActor(AActor* thisActor, const AActor* targetActor, float deltaTime, float rotateSpeed)
{
	// Get rotator toward target actor
	const FRotator toTargetActorRotator = GetRotatorTowardActor(thisActor, targetActor);

	// Interpolate from the current rotation to the target rotation
	const FRotator newRotator = FMath::RInterpTo(thisActor->GetActorRotation(), toTargetActorRotator, deltaTime, rotateSpeed);

	// Set the actor's rotation to the interpolated rotation
	thisActor->SetActorRotation(newRotator);	
}

/*
 * Get distance in float between the first actor and second actor
 */
float UActorUtilities::GetDistanceBetweenActor(const AActor* firstActor, const AActor* secondActor)
{
	return (firstActor->GetActorLocation() - secondActor->GetActorLocation()).Size();	
}

/*
 * Return location before target actor
 */
FVector UActorUtilities::GetLocationBeforeActor(const AActor* thisActor, const AActor* targetActor, float offsetUntilActor)
{
	const FVector towardMainActor = (thisActor->GetActorLocation() - targetActor->GetActorLocation()).GetSafeNormal();
	const FVector locationBeforeActor = targetActor->GetActorLocation() + towardMainActor * offsetUntilActor;

	return locationBeforeActor;
}
#pragma endregion ActorUtilities



#pragma region MathUtilities
/*
 * Calculate actor's future location after n seconds based on current velocity
 */
FVector UMathUtilities::CalculateFutureActorLocation(const AActor* movingActor, float secondLater)
{
	// Get current velocity in 2D and calculate future location
	const FVector currentVelocity = movingActor->GetVelocity() * FVector(1.0f, 1.0f, 0.0f); // Ignore Z axis velocity

	// Result location = (velocity * time) + currentLocation
	return movingActor->GetActorLocation() + currentVelocity * secondLater;
}

/*
 * Calculate actor's future location after n seconds based on current velocity
 * with offset
 */
FVector UMathUtilities::CalculateFutureActorLocation(const AActor* thisActor, const AActor* movingActor
	, float secondLater = 0.0f, float offset = 0.0f)
{
	// Calculate future location of moving actor
	FVector targetLocation = CalculateFutureActorLocation(movingActor, secondLater);

	// Get offset before reaching moving actor and return it
	FVector vectorFromTarget = (thisActor->GetActorLocation() - targetLocation).GetSafeNormal();
	vectorFromTarget *= offset;
	targetLocation = targetLocation + vectorFromTarget;
	
	return targetLocation;
}

/*
 * Calculate midpoint between 2 locations
 */
FVector UMathUtilities::CalculateMidpoint(const FVector startLocation, const FVector targetLocation, float midpointRatio)
{
	return startLocation + (targetLocation - startLocation) * midpointRatio;
}

/*
 * Calculate curve point based on midpoint
 */
FVector UMathUtilities::GetCurveControlPoint(const FVector midpointLocation, const FVector targetLocation, float angle, float curveAmplitude)
{
	// Find rotation for object at start location to point at target location
	const FRotator toTargetRotator = UKismetMathLibrary::FindLookAtRotation(midpointLocation, targetLocation);

	// Break rotator into axes
	FVector axisX, axisY, axisZ;
	UKismetMathLibrary::BreakRotIntoAxes(toTargetRotator, axisX, axisY,axisZ);
	
	// Returns result of vector rotated by AngleDeg around Axis
	const FVector rotatedVector = UKismetMathLibrary::RotateAngleAxis(axisY * curveAmplitude, angle, axisX);

	return midpointLocation + rotatedVector;
}

/*
 * Move actor toward target location in curve trajectory
 * @param curvePointInterpSpeed determine how fast curve point moving
 *							    toward target location
 * @param actorInterpSpeed      determine how fast actor following
 *							    curve point					    
 */
void UMathUtilities::CurveToLocation(AActor* actorToMove, FVector& curvePoint, const FVector targetLocation
	, float deltaTime, float curvePointInterpSpeed, float actorInterpSpeed)
{
	// Firstly, we interpolate curve point toward target location
	// To update curvePoint we need to pass it by reference!
	curvePoint = FMath::VInterpTo(curvePoint, targetLocation, deltaTime, curvePointInterpSpeed);
	// Then we interpolate actor location toward curve point consecutively
	InterpolateToLocation(actorToMove, curvePoint, deltaTime, actorInterpSpeed);
}

// Smoothly move actor toward target location
void UMathUtilities::InterpolateToLocation(AActor* actorToMove, const FVector targetLocation, float deltaTime,
	float actorInterpSpeed)
{
	const FVector newLocation = FMath::VInterpTo(actorToMove->GetActorLocation(), targetLocation, deltaTime, actorInterpSpeed);
	actorToMove->SetActorLocation(newLocation);
}

float UMathUtilities::ScaleValueInOtherRange(float value, float oldRangeMin, float oldRangeMax, float newRangeMin,
	float newRangeMax)
{
	// 値が元の範囲の何割かを取得
	const float percentage = FMath::GetRangePct(oldRangeMin, oldRangeMax, value);

	// 取得した割合を新しい範囲に適用
	const float scaledValue = FMath::Lerp(newRangeMin, newRangeMax, percentage);

	return scaledValue;
}
#pragma endregion MathUtilities



#pragma region EnumUtilities

#pragma endregion EnumUtilities


#pragma region UIUtilities

void UUIUtilities::SetFocusUI(UObject* contextObject)
{
	// プレイヤーのコントローラーを取得
	auto playerController = UGameplayStatics::GetPlayerController(contextObject, 0);
	if(playerController)
	{
		// プレイヤーのコントローラーの入力モードをUIモードにする
		FInputModeUIOnly InputMode;
		playerController->SetInputMode(InputMode);

		// マウスカーソルを表示する
		playerController->bShowMouseCursor = true;
	}
}

void UUIUtilities::SetFocusInGame(UObject* contextObject)
{
	// プレイヤーのコントローラーを取得
	auto playerController = UGameplayStatics::GetPlayerController(contextObject, 0);
	if(playerController)
	{
		// プレイヤーのコントローラーの入力モードをゲームモードに戻す
		playerController->bShowMouseCursor = false;
		FInputModeGameOnly inputMode;
		playerController->SetInputMode(inputMode);
	}
}

void UUIUtilities::SetFocusUIAndInGame(UObject* contextObject)
{
	// プレイヤーのコントローラーを取得
	auto playerController = UGameplayStatics::GetPlayerController(contextObject, 0);
	if(playerController)
	{
		// プレイヤーのコントローラーの入力モードをゲーム&UIモードにする
		playerController->bShowMouseCursor = true;
		FInputModeGameAndUI inputMode;
		playerController->SetInputMode(inputMode);
	}
}

void UUIUtilities::InterpProgressBar(UProgressBar* progressBar, float targetValue, float InDeltaTime, float interpSpeed)
{
	const float currentValue = progressBar->GetPercent();
	const float newValue = FMath::FInterpTo(currentValue, targetValue, InDeltaTime, interpSpeed);
	progressBar->SetPercent(newValue);
}

void UUIUtilities::UpdateTextBlock(UTextBlock* textBlock, int value)
{
	UpdateTextBlock(textBlock, static_cast<float>(value));
}

void UUIUtilities::UpdateTextBlock(UTextBlock* textBlock, float value)
{
	const FText valueText = FText::AsNumber(value);
	textBlock->SetText(valueText);
}

void UUIUtilities::SetButtonStyle(UButton* button, UTexture2D* buttonTexture)
{
	FSlateBrush buttonBrush;
	buttonBrush.SetResourceObject(buttonTexture);
	buttonBrush.ImageSize = FVector2D(buttonTexture->GetSizeX(),
		buttonTexture->GetSizeY());
	
	FButtonStyle ButtonStyle = button->GetStyle();
	ButtonStyle.SetNormal(buttonBrush);
	ButtonStyle.SetHovered(buttonBrush);
	ButtonStyle.SetPressed(buttonBrush);
	button->SetStyle(ButtonStyle);
}
#pragma endregion UIUtilities

#pragma region MasterUtilities
UDataStorage* UMasterUtilities::GetDataStorage(const UObject* contextObject)
{
	if(auto gameInstance = contextObject->GetWorld()->GetGameInstance() ?
		Cast<IManageSingleton>(contextObject->GetWorld()->GetGameInstance()) : nullptr)
	{
		if(gameInstance->GetDataStorage_Implementation())
		{
			return gameInstance->GetDataStorage_Implementation();
		}
	}

	return nullptr;
}

UQuestManager* UMasterUtilities::GetQuestManager(const UObject* contextObject)
{
	if(auto gameInstance = contextObject->GetWorld()->GetGameInstance() ?
	Cast<IManageSingleton>(contextObject->GetWorld()->GetGameInstance()) : nullptr)
	{
		if(auto questManagerInstance = gameInstance->GetQuestManager_Implementation())
		{
			return questManagerInstance;
		}
	}

	return nullptr;
}
#pragma endregion MasterUtilities
