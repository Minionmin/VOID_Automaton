// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "VOIDCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VOID_Automaton/DebugMacros.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	sphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SetRootComponent(sphereCollision);
	sphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // To prevent collision at the start (on spawn)
	sphereCollision->SetCollisionObjectType(PROJECTILE_COLLISION_CHANNEL);
	sphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	sphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	sphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	sphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	projectileMovementComponent->Velocity = FVector::Zero(); // will custom our own movement
	projectileMovementComponent->ProjectileGravityScale = 0.f; // is not affected by gravity
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereOverlap);
}


void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
                                      UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
}

void AProjectileBase::TurnOnCollision()
{
	sphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AProjectileBase::TurnOffCollision()
{
	sphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Set projectile velocity with offset option
void AProjectileBase::SetProjectileVelocity(FVector targetLocation, float velocity, bool bShouldOffset, float offset)
{
	// Calculate direction
	auto direction = targetLocation - GetActorLocation();

	// Offset velocity
	if(bShouldOffset)
	{
		// Get right vector to target location
		auto rightVector = FVector::CrossProduct(direction, FVector::UpVector).GetSafeNormal();
		direction += rightVector * FMath::FRandRange(-offset, offset);
	}
	direction = direction.GetSafeNormal();

	// Set projectile velocity
	projectileMovementComponent->Velocity = direction * velocity;
}

FGameplayTagContainer AProjectileBase::GetGameplayTagContainer_Implementation()
{
	return actorTagsContainer;
}
