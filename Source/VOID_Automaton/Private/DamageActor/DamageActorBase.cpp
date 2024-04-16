// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageActor/DamageActorBase.h"

#include "VOIDCharacter.h"
#include "Components/BoxComponent.h"
#include "Engine/StreamableManager.h"
#include "Interfaces/Hitable.h"
#include "Kismet/GameplayStatics.h"


ADamageActorBase::ADamageActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>("Sphere Collision");
	SetRootComponent(boxCollision);
	boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	boxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	boxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	boxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	boxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADamageActorBase::BeginPlay()
{
	Super::BeginPlay();

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADamageActorBase::OnBoxOverlap);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ADamageActorBase::OnBoxEndOverlap);

	FTimerHandle selfDestructTimer;
	GetWorld()->GetTimerManager().SetTimer(selfDestructTimer, [&]()
	{
		Destroy();
	}, lifeTime, false);
}

void ADamageActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageActorBase::OnBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                    UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Do not apply damage to the instigator
	if(GetInstigator())
	{
		if(otherActor == GetInstigator()) return;
	}
	
	// ** Actor that implemented IHitableInterface ** //
	if(IHitable* hitableActor = Cast<IHitable>(otherActor))
	{
		hitableActor->GetHit_Implementation(sweepResult);
	}

	// ** Actor is player ** //
	if(AVOIDCharacter* playerRef = Cast<AVOIDCharacter>(otherActor))
	{
		// Do not spawn ImpactVFX and apply damage to player if the player is in invincible frame
		if(playerRef->IsInIFrame())
		{
			return;
		}
	}

	// No Instigator controller
	UGameplayStatics::ApplyDamage(otherActor, baseDamage, GetInstigatorController(), this, UDamageType::StaticClass());
}

void ADamageActorBase::OnBoxEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
}

FGameplayTagContainer ADamageActorBase::GetGameplayTagContainer_Implementation()
{
	return actorTagsContainer;
}