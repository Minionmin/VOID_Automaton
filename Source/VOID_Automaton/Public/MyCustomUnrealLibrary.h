// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ManageSingleton.h"
#include "Managers/QuestManager.h"
#include "MyCustomUnrealLibrary.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class VOID_AUTOMATON_API UMyCustomUnrealLibrary: public UObject
{
	GENERATED_BODY()

public:

	
};

UCLASS()
class VOID_AUTOMATON_API UActorUtilities : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	static FRotator GetRotatorTowardActor(const AActor* thisActor, const AActor* targetActor, bool bVerticalComponent = false);
	static FRotator GetRotatorTowardActor(const FVector& thisLocation, const FVector& targetLocation, bool bVerticalComponent = false);
	UFUNCTION(BlueprintCallable)
	static void FaceActor(AActor* thisActor, const AActor* targetActor, float deltaTime, float rotateSpeed);
	UFUNCTION(BlueprintCallable)
	static float GetDistanceBetweenActor(const AActor* firstActor, const AActor* secondActor);
	UFUNCTION(BlueprintCallable)
	static FVector GetLocationBeforeActor(const AActor* thisActor, const AActor* targetActor, float offsetUntilActor);
};

UCLASS()
class VOID_AUTOMATON_API UMathUtilities : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static FVector CalculateFutureActorLocation(const AActor* thisActor, const AActor* movingActor, float secondLater, float offset);
	static FVector CalculateFutureActorLocation(const AActor* movingActor, float secondLater);
	UFUNCTION(BlueprintCallable)
	static FVector CalculateMidpoint(const FVector startLocation, const FVector targetLocation, float midpointRatio);
	UFUNCTION(BlueprintCallable)
	static FVector GetCurveControlPoint(const FVector midpointLocation, const FVector targetLocation, float angle, float curveAmplitude);
	UFUNCTION(BlueprintCallable)
	static void CurveToLocation(AActor* actorToMove, FVector& curvePoint, const FVector targetLocation
		, float deltaTime, float curvePointInterpSpeed, float actorInterpSpeed);
	UFUNCTION(BlueprintCallable)
	static void InterpolateToLocation(AActor* actorToMove, const FVector targetLocation , float deltaTime, float actorInterpSpeed);
	UFUNCTION(BlueprintCallable)
	static float ScaleValueInOtherRange(float value, float oldRangeMin, float oldRangeMax,
		float newRangeMin, float newRangeMax);
};

UCLASS()
class VOID_AUTOMATON_API UEnumUtilities : public UObject
{
	GENERATED_BODY()

public:

	// Function to get a random enum value from an enum class
	template<typename T>
	UFUNCTION(BlueprintCallable)
	static T GetRandomEnumValue()
	{
		// Check if the template parameter is an enum
		static_assert(std::is_enum<T>::value, "Template parameter must be an enum type.");
		
		int32 enumValueCount = static_cast<int32>(T::enumCount); // User needs to put enumCount at the end of enum
		return static_cast<T>(FMath::RandRange(1, enumValueCount - 1));
	}
};

UCLASS()
class VOID_AUTOMATON_API UUIUtilities : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void SetFocusUI(UObject* contextObject);
	UFUNCTION(BlueprintCallable)
	static void SetFocusInGame(UObject* contextObject);
	UFUNCTION(BlueprintCallable)
	static void SetFocusUIAndInGame(UObject* contextObject);
	
	UFUNCTION()
    static void InterpProgressBar(UProgressBar* progressBar, float targetValue,float InDeltaTime, float interpSpeed);

	UFUNCTION()
	static void UpdateTextBlock(UTextBlock* textBlock, int value);
	static void UpdateTextBlock(UTextBlock* textBlock, float value);
	UFUNCTION()
	static void SetButtonStyle(UButton* button, UTexture2D* buttonTexture);
};

UCLASS()
class VOID_AUTOMATON_API UMasterUtilities : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	static class UDataStorage* GetDataStorage(const UObject* contextObject);
	UFUNCTION()
	static UQuestManager* GetQuestManager(const UObject* contextObject);
};