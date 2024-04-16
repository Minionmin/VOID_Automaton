// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/HomingProjectile.h"
#include "StateManagerComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VOID_Automaton/DebugMacros.h"

AHomingProjectile::AHomingProjectile()
{
	// State Manager
	stateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("Projectile State Manager"));
}

void AHomingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHomingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	stateManager->InitStateManager();
}

void AHomingProjectile::OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	Super::OnSphereOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
}

void AHomingProjectile::StartHomingRangeAttackFanShape(AActor* spawnedBy, AActor* targetActor
	, TSubclassOf<AHomingProjectile> projectileSubclass, int32 projectileCount, float fanRadius
	, float zOffset, float projectileSpeed, FRotator projectileRotation, bool bShouldSpawn)
{
	// Get world
	UWorld* world = spawnedBy->GetWorld();
	
	// ** Setup spawnLocation and angleDelta for equal interval fan shape **
	const FVector spawnLocation = spawnedBy->GetActorLocation() + FVector(0.0f, 0.0f, zOffset);
	float angleDelta;
	if(projectileCount % 2 == 1 && projectileCount > 1)
	{
		angleDelta = fanRadius / (projectileCount - 1);
	}
	else
	{
		angleDelta = fanRadius / projectileCount;
	}
	
	// ** Projectile count is odd **
	// We spawn the middle projectile separately
	if(projectileCount % 2 == 1)
	{
		const FVector dirVelocity = FVector(0.f , FMath::Sin(0.f), FMath::Cos(0.f));
		
		if(projectileSubclass)
		{
			AHomingProjectile* newProjectile =  world->SpawnActor<AHomingProjectile>(projectileSubclass,
				spawnLocation + dirVelocity * 300.f, projectileRotation);
			if(newProjectile)
			{
				newProjectile->GetProjectileMovementComponent()->Velocity = dirVelocity * projectileSpeed;
				newProjectile->SetProjectileTarget(targetActor);
				newProjectile->SetProjectileOwner(spawnedBy);
				newProjectile->bShouldSpawnProjectile = bShouldSpawn;
				newProjectile->TurnOnCollision();
			}
		}
	}
	
	// ** Spawn projectiles for both sides (+ and - sides) with calculated interval **
	for (int32 i = 1; i <= projectileCount / 2; i++)
	{
		// ** Get angle abs value before applying +/- **
		const float angle = i * angleDelta; 
		const float AngleRadians = FMath::DegreesToRadians(angle);
	
		const FVector positiveDirVelocity = FVector(0.f , FMath::Sin(AngleRadians), FMath::Cos(AngleRadians));
		const FVector negativeDirVelocity = FVector(0.f , FMath::Sin(AngleRadians) * -1.f, FMath::Cos(AngleRadians));
	
		// + side
		AHomingProjectile* newPositiveProjectile = world->SpawnActor<AHomingProjectile>(projectileSubclass,
			spawnLocation + positiveDirVelocity * 300.f, projectileRotation);
		if(newPositiveProjectile)
		{
			newPositiveProjectile->GetProjectileMovementComponent()->Velocity = positiveDirVelocity * projectileSpeed;
			newPositiveProjectile->SetProjectileTarget(targetActor);
			newPositiveProjectile->SetProjectileOwner(spawnedBy);
			newPositiveProjectile->bShouldSpawnProjectile = bShouldSpawn;
			newPositiveProjectile->TurnOnCollision();
		}
		
		// - side
		AHomingProjectile* newNegativeProjectile = world->SpawnActor<AHomingProjectile>(projectileSubclass,
			spawnLocation + negativeDirVelocity * 300.f, projectileRotation);
		if(newPositiveProjectile)
		{
			newNegativeProjectile->GetProjectileMovementComponent()->Velocity = negativeDirVelocity * projectileSpeed;
			newNegativeProjectile->SetProjectileTarget(targetActor);
			newNegativeProjectile->SetProjectileOwner(spawnedBy);
			newNegativeProjectile->bShouldSpawnProjectile = bShouldSpawn;
			newNegativeProjectile->TurnOnCollision();
		}
	}
}

FGameplayTagContainer AHomingProjectile::GetGameplayTagContainer_Implementation()
{
	return Super::GetGameplayTagContainer_Implementation();
}