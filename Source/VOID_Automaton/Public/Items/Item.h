#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HasInstructionWidget.h"
#include "Item.generated.h"

class USphereComponent;
class UBoxComponent;
class UWidgetComponent;

UCLASS()
class VOID_AUTOMATON_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItem();
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	virtual void BeginPlay() override;


protected:

	/*
		Components
	*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* itemMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* collisionBox;
};
