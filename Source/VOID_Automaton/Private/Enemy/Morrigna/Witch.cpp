// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Morrigna/Witch.h"

#include "AITypes.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TagList.h"
#include "VOIDCharacter.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Projectile/CascadeProjectileBase.h"
#include "Projectile/HomingProjectile.h"
#include "Projectile/NiagaraProjectileBase.h"
#include "VOID_Automaton/DebugMacros.h"

AWitch::AWitch()
{
	// Swipe projectile spawn point (scene component)
	swipeProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>("Swipe Projectile Spawn Location");
	swipeProjectileSpawnLocation->SetupAttachment(GetRootComponent());
	
	// Hit detection for melee attack
	meleeAttackBoxCollision = CreateDefaultSubobject<UBoxComponent>("Melee Attack Box Collision");
	meleeAttackBoxCollision->SetupAttachment(GetRootComponent());
	meleeAttackBoxCollision->CanCharacterStepUpOn = ECB_No;
	meleeAttackBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	meleeAttackBoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	meleeAttackBoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	meleeAttackBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Hit detection for scream attack
	screamAttackBoxCollision = CreateDefaultSubobject<UBoxComponent>("Scream Attack Box Collision");
	screamAttackBoxCollision->SetupAttachment(GetRootComponent());
	screamAttackBoxCollision->CanCharacterStepUpOn = ECB_No;
	screamAttackBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	screamAttackBoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	screamAttackBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Moving trail vfx
	movingTrailVFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("MovingTrail");
	movingTrailVFXComponent->SetupAttachment(GetRootComponent());
	
	// Get default teleport velocity
	originalTeleportVelocity = teleportVelocity;
}

void AWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWitch::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind delegate and set hitbox to no collision (turn on using animmontage)
	DeactivateHitbox(screamAttackBoxCollision);
	DeactivateHitbox(meleeAttackBoxCollision);
	
	screamAttackBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWitch::OnScreamAttackBoxOverlap);
	meleeAttackBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWitch::OnMeleeAttackBoxOverlap);

	// VFX during punch combo
	rightPunchVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(meleeVFX, GetMesh(), FName("RightHandVFXSocket")
		,FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);
	leftPunchVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(meleeVFX, GetMesh(), FName("LeftHandVFXSocket")
		,FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);
	rightFootVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(meleeVFX, GetMesh(), FName("RightFootVFXSocket")
	,FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, false);

	rightPunchVFXComponent->Deactivate();
	leftPunchVFXComponent->Deactivate();
	rightFootVFXComponent->Deactivate();
}

void AWitch::NormalTeleport(const FVector& destination)
{
	// Spawn teleportVFX at both old location and new location
	if(teleportVFX)
	{
		SpawnNormalTeleportEffectAt(GetActorLocation());
		// Spawn effect on teleport ended(arrived)
		OnTeleportEnd.RemoveDynamic(this, &AWitch::SpawnNormalTeleportEffectAt);
		OnTeleportEnd.AddDynamic(this, &AWitch::SpawnNormalTeleportEffectAt);
	}

	Teleport(destination);
}

void AWitch::CombatTeleport(const FVector& destination)
{
	// Spawn combat version of teleportVFX at old location
	if(combatTeleportVFX)
	{
		AWitch::SpawnCombatTeleportEffectAt(GetActorLocation());
		// Spawn effect on teleport ended(arrived)
		OnTeleportEnd.AddDynamic(this, &AWitch::SpawnCombatTeleportEffectAt);
	}
	
	Teleport(destination);
}

void AWitch::Teleport(const FVector& destination)
{
	auto movementComponent = GetCharacterMovement();

	// Move fast so it looks like teleporting
	movementComponent->SetMovementMode(MOVE_Flying);
	movementComponent->MaxFlySpeed = teleportVelocity;
	movementComponent->MaxAcceleration = 999999.f;
	
	// Can teleport through pawn
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// Move to destination
	FPathFollowingRequestResult moveResult;

	FScriptDelegate movementCompleteDelegate;
	movementCompleteDelegate.BindUFunction(this, FName("TeleportEnd"));
	
	//InitiateMoveTo(destination, 100.f, moveResult);
	InitiateMoveTo(destination, 25.f, movementCompleteDelegate, moveResult);
}

void AWitch::TeleportEnd()
{
	auto movementComponent = GetCharacterMovement();

	// Set all the variables back to normal
	movementComponent->SetMovementMode(MOVE_Walking);
	movementComponent->MaxFlySpeed = originalTeleportVelocity;
	movementComponent->MaxAcceleration = 1500.f;
	
	// Stop teleport through pawn
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	// Remove any existing bindings to OnMoveCompleted
	enemyAIController->ReceiveMoveCompleted.Clear();

	// Broadcast on teleport end
	OnTeleportEnd.Broadcast(GetActorLocation());

	// Then Unbind all
	OnTeleportEnd.Clear();
}

void AWitch::SpawnCombatTeleportEffectAt(FVector location)
{
	UParticleSystemComponent* teleportTrail = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		combatTeleportVFX,
		location,
		GetActorRotation()
	);
}

void AWitch::SpawnNormalTeleportEffectAt(FVector location)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), teleportVFX, location);
}

void AWitch::InitiateMoveTo(const FVector& targetLocation, const float acceptableRadius)
{
	FAIMoveRequest moveRequest;
	moveRequest.SetAcceptanceRadius(acceptableRadius);
	moveRequest.SetGoalLocation(targetLocation);
	
	// Get the AI controller and initiate MoveTo operation
	if (enemyAIController)
	{
		// Start moving
		enemyAIController->MoveTo(moveRequest);
	}	
}

void AWitch::InitiateMoveTo(const FVector& targetLocation, const float acceptableRadius,
	const FScriptDelegate& movementCompleteDelegate, FPathFollowingRequestResult& outResult)
{
	FAIMoveRequest moveRequest;
	moveRequest.SetAcceptanceRadius(acceptableRadius);
	moveRequest.SetGoalLocation(targetLocation);

	// Get the AI controller and initiate MoveTo operation
	if (enemyAIController)
	{
		// Remove any existing bindings to OnMoveCompleted
		enemyAIController->ReceiveMoveCompleted.Clear();
		
		// Bind the callback function to the OnMoveCompleted delegate
		enemyAIController->ReceiveMoveCompleted.Add(movementCompleteDelegate);

		// Start moving
		outResult = enemyAIController->MoveTo(moveRequest);
	}	
}

void AWitch::SpawnSwipeProjectile()
{
	// Summon projectile off swipe attack (animnotify)
	if(swipeProjectileClass == nullptr) return;

	// Spawn projectile using spawn location
	const FVector leftSpawnLocation = swipeProjectileSpawnLocation->GetComponentLocation() + GetActorRightVector() * -1.f * centerOffset;
	const FVector rightSpawnLocation = swipeProjectileSpawnLocation->GetComponentLocation() + GetActorRightVector() * centerOffset;
	
	// Spawn both projectiles
	AProjectileBase* leftProjectile = GetWorld()->SpawnActor<AProjectileBase>(swipeProjectileClass,
		leftSpawnLocation, GetActorRotation());
	AProjectileBase* rightProjectile = GetWorld()->SpawnActor<AProjectileBase>(swipeProjectileClass,
		rightSpawnLocation, GetActorRotation());

	// Set both projectiles' owner
	leftProjectile->SetProjectileOwner(this);
	rightProjectile->SetProjectileOwner(this);
	
	// Set both projectiles' velocity to match actor's forward vector
	leftProjectile->GetProjectileMovementComponent()->Velocity = GetActorForwardVector() * swipeProjectileSpeed;
	rightProjectile->GetProjectileMovementComponent()->Velocity = GetActorForwardVector() * swipeProjectileSpeed;

	// Then turn on both projectile's collision
	leftProjectile->TurnOnCollision();
	rightProjectile->TurnOnCollision();
}

void AWitch::SpawnNormalProjectile()
{
	if(normalProjectileClass == nullptr) return;
	
	// ** Setup spawnLocation **
	const FVector spawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, homingZOffset);

	// Spawn the projectile
	AProjectileBase* newProjectile =  GetWorld()->SpawnActor<AProjectileBase>(normalProjectileClass,
	spawnLocation, GetActorRotation());

	// Set projectile direction
	auto playerRef =UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Set projectile owner
	newProjectile->SetProjectileOwner(this);
	
	// Set projectile velocity
	newProjectile->SetProjectileVelocity(playerRef->GetActorLocation(), swipeProjectileSpeed, true, projectileDirectionOffset);

	// Turn on collision
	newProjectile->TurnOnCollision();
}

void AWitch::SpawnHomingRangeProjectile()
{
	// Make homing projectile aimed toward player
	auto playerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	AHomingProjectile::StartHomingRangeAttackFanShape(this, playerRef,
		homingProjectileClass, homingProjectileCount, homingFanRadius, homingZOffset,
		homingProjectileSpeed, GetActorForwardVector().Rotation(), bhasEnteredSecondPhase);
}

void AWitch::GetHit_Implementation(FHitResult hitResult)
{
	Super::GetHit_Implementation(hitResult);
}

float AWitch::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	// Don't take damage if already dead
	if(bIsDead) return 0.f;
	
	if(attributes)
	{
		// Clamp between 0 and maxHealth
		attributes->ReceiveDamage(DamageAmount);

		// 2nd Phase at 50% health
		if(attributes->GetHealthPercentage() <= 0.5f && !bhasEnteredSecondPhase)
		{
			// Make projectile harder to dodge
			bhasEnteredSecondPhase = true;

			// VFX
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), secondPhaseVFX, GetActorLocation());
			
			// SFX
			//UGameplayStatics::PlaySound2D(GetWorld(), secondPhaseScreechSFX);
		}

		// Broadcast event
		OnTakingDamage.Broadcast();

		// Check if dead
		if(attributes->HasNoHealth())
		{
			Die();
		}
	}
	
	return DamageAmount;
}

void AWitch::Die()
{
	Super::Die();

	// Switch to dead state
	stateManager->SwitchStateByKey("WitchDead");
	
	// Append camera shake to player
	if(deathCameraShakeClass)
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(deathCameraShakeClass);	
	}

	// Unbind all events
	OnTeleportEnd.Clear();
}

void AWitch::OnScreamAttackBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Only hit object once per attack
	if(hitActors.Contains(otherActor)) return;
	hitActors.AddUnique(otherActor);
	
	// Only deal damage if it's player
	if(IHasGameplayTag* gameplayTagActor = Cast<IHasGameplayTag>(otherActor))
	{
		const FGameplayTagContainer tagContainer = gameplayTagActor->GetGameplayTagContainer_Implementation();
		const FGameplayTag tagToCheck = FGameplayTag::RequestGameplayTag(TagList::PLAYER);
		if(tagToCheck.IsValid() && tagContainer.HasTag(tagToCheck))
		{
			// Also launch player in the opposite direction
			if(auto playerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				const auto playerLocation = playerRef->GetActorLocation();
				auto playerOppositeVector = (playerLocation - GetActorLocation()).GetSafeNormal();
				playerOppositeVector.Z = 0.f;
				const auto launchDir = playerOppositeVector * screamKnockbackDistance + screamKnockbackZDistance;
				playerRef->LaunchCharacter(launchDir, false, false);

				// Also append camera shake to player
				if(screamCamShakeClass)
				{
					UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(screamCamShakeClass);	
				}

				// Use in state machine
				OnDamagedPlayer.Broadcast();
			}

			
			// Apply dmg to player
			UGameplayStatics::ApplyDamage(otherActor, screamAttackDamage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}

	hitActors.Empty();
}

void AWitch::OnMeleeAttackBoxOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Only hit object once per attack
	if(hitActors.Contains(otherActor)) return;
	hitActors.AddUnique(otherActor);
	
	// Only deal damage if it's player
	if(IHasGameplayTag* gameplayTagActor = Cast<IHasGameplayTag>(otherActor))
	{
		const FGameplayTagContainer tagContainer = gameplayTagActor->GetGameplayTagContainer_Implementation();
		const FGameplayTag tagToCheck = FGameplayTag::RequestGameplayTag(TagList::PLAYER);
		if(tagToCheck.IsValid() && tagContainer.HasTag(tagToCheck))
		{
			// Also append camera shake to player
			if(meleeCamShakeClass)
			{
				UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(meleeCamShakeClass);	
			}

			// Use in state machine
			OnDamagedPlayer.Broadcast();
			
			// Apply dmg to player
			UGameplayStatics::ApplyDamage(otherActor, meleeAttackDamage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}

	hitActors.Empty();
}

void AWitch::ActivateHitbox(UBoxComponent* hitbox)
{
	hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWitch::DeactivateHitbox(UBoxComponent* hitbox)
{
	hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWitch::EnableTrailVFX()
{
	// Enable moving trail
	movingTrailVFXComponent->Activate();
}

void AWitch::DisableTrailVFX()
{
	// Disable moving trail
	movingTrailVFXComponent->Deactivate();
}

