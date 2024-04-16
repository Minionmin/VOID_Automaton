// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/NiagaraProjectileBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "VOIDCharacter.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Hitable.h"
#include "Kismet/GameplayStatics.h"

// When you create an instance of a derived class, the constructor of the base class is called first,
// then the constructor of the derived class
ANiagaraProjectileBase::ANiagaraProjectileBase()
{
	projectileNiagara = CreateDefaultSubobject<UNiagaraComponent>("Projectile Niagara");
	projectileNiagara->SetupAttachment(sphereCollision);
}

void ANiagaraProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANiagaraProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void ANiagaraProjectileBase::OnSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Do nothing if actor is the owner
	if(projectileOwner && otherActor == projectileOwner) return;
	
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

		// Shake player camera if conditions are met
		if(bShouldShakeCamera && projectileCamShakeClass)
		{
			UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(projectileCamShakeClass);	
		}
		
		// No Instigator controller
		UGameplayStatics::ApplyDamage(otherActor, baseDamage, GetInstigatorController(), this, UDamageType::StaticClass());
	}
	
	// ** VFX ** //
	if(impactVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), impactVFX, sweepResult.Location);
	}
	
	Destroy();
}

FGameplayTagContainer ANiagaraProjectileBase::GetGameplayTagContainer_Implementation()
{
	return Super::GetGameplayTagContainer_Implementation();
}