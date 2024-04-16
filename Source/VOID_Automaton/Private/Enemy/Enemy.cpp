// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "AIController.h"
#include "DelayAction.h"
#include "MyCustomUnrealLibrary.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAIController.h"
#include "HUDs/EnemyHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AudioManager.h"
#include "VOID_Automaton/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Use Mesh colliders to get trace result
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Use capsule for world static collision
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
	
	// Enemy attributes (hp)
	attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// State Manager
	stateManager = CreateDefaultSubobject<UStateManagerComponent>(TEXT("State Manager"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize AIController
	if(GetController())
	{
		enemyAIController = Cast<AEnemyAIController>(GetController());
	}

	// Initialize animInstance
	animInstance = GetMesh()->GetAnimInstance();
	
	// Create & Initialize enemy health bar on player's screen
	enemyHealthBarWidget = CreateWidget<UEnemyHealthBar>(UGameplayStatics::GetPlayerController(GetWorld(),0), enemyHealthBarWidgetClass);
	if(enemyHealthBarWidget)
	{
		enemyHealthBarWidget->AddToViewport();
		enemyHealthBarWidget->SetEnemyName(enemyName);
	}

	// Stats
	if(attributes)
	{
		attributes->OnHealthValueChanged.AddDynamic(enemyHealthBarWidget, &UEnemyHealthBar::SetHealthBarPercentage);
	}

	// Quest Managerにイベントを登録する
	if(auto questManager = UMasterUtilities::GetQuestManager(this))
	{
		OnEnemyDead.AddDynamic(questManager, &UQuestManager::ShowQuestResultWidget);
	}
	
	// Init State Manager
	stateManager->InitStateManager();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ** Update dmgIndicator location ** //
	UpdateDmgIndicator();
}

void AEnemy::GetHit_Implementation(FHitResult hitResult)
{
	// ** Play SFX ** //
	if(impactSFX)
	{
		if(const auto singletonManager = Cast<IManageSingleton>(GetGameInstance()))
		{
			if(const auto audioManager = singletonManager->GetAudioManager_Implementation())
			{
				audioManager->PlaySFX(impactSFX, this);
			}
		}
	}

	// ** Play VFX ** //
	if(impactParticleVFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactParticleVFX, hitResult.Location, FRotator(0.f), true);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(attributes)
	{
		// Clamp between 0 and maxHealth
		attributes->ReceiveDamage(DamageAmount);
		
		// Die if health is 0
		if(attributes->HasNoHealth())
		{
			Die();
		}
	}
	
	return DamageAmount;
}

void AEnemy::Die()
{
	bIsDead = true;
	// Turn off visibility collision to avoid any further damage
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	// Tell quest manager that this enemy is dead
	OnEnemyDead.Broadcast();
	// Unbind all delegates
	OnEnemyDead.Clear();
	OnTakingDamage.Clear();
}


void AEnemy::StoreDmgIndicator(UUserWidget* dmgIndicatorWidget, FVector location)
{
	// Store widget and it's location later for removing it from screen
	// The reason we create many widgets because each number (dmg indicator) require their own
	// viewport to show itself to the screen
	dmgIndicators.Add(dmgIndicatorWidget, location);

	// We need to bind the function to delegate in case there is at least one input parameter
	// Also the reason we didn't create timer handler in .h file because we cannot handle all the calls
	// by just one timer handler (there are many widgets to destroy)
	FTimerHandle dmgIndicatorTimer;
	FTimerDelegate dmgIndicatorDelegate;
	dmgIndicatorDelegate.BindUFunction(this, FName("DestroyDmgIndicator"), dmgIndicatorWidget);
	GetWorld()->GetTimerManager().SetTimer(dmgIndicatorTimer, dmgIndicatorDelegate, dmgIndicatorDestroyTime, false);
}

void AEnemy::DestroyDmgIndicator(UUserWidget* dmgIndicatorWidgetToDestroy)
{
	// Destroy dmgIndicatorWidget after a short amount of time
	dmgIndicators.Remove(dmgIndicatorWidgetToDestroy);
	dmgIndicatorWidgetToDestroy->RemoveFromParent(); // will be removed from the viewport automatically
}

void AEnemy::UpdateDmgIndicator()
{
	for(auto& dmgPair : dmgIndicators)
	{
		UUserWidget* dmgIndicator = dmgPair.Key;
		const FVector location = dmgPair.Value;
		FVector2D screenPos;
		UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			location, screenPos);
		dmgIndicator->SetPositionInViewport(screenPos);
	}
}

