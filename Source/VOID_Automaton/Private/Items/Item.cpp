
#include "Items/Item.h"

#include "Components/BoxComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	itemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	itemMesh->SetSimulatePhysics(true);
	itemMesh->SetEnableGravity(true);
	itemMesh ->SetCollisionResponseToAllChannels(ECR_Block);
	itemMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	itemMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	RootComponent = itemMesh;

	// Actual collision
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	collisionBox->SetupAttachment(itemMesh);
	collisionBox->CanCharacterStepUpOn = ECB_No;
	collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	collisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
