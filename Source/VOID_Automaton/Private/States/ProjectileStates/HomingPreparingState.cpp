// Fill out your copyright notice in the Description page of Project Settings.


#include "States/ProjectileStates/HomingPreparingState.h"

#include "StateManagerComponent.h"
#include "Projectile/HomingProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "VOID_Automaton/DebugMacros.h"

void UHomingPreparingState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	// Delay projectile before launching
	FTimerHandle homingTimer;
	GetWorld()->GetTimerManager().SetTimer(homingTimer, [this, stateOwner]()
	{
		if(homingProjectileRef && homingProjectileRef->GetProjectileTarget())
		{
			// Check if local target is nullptr or not
			if(!target)
			{
				target = homingProjectileRef->GetProjectileTarget();
			}
						
			// ** Spawn more projectiles first if condition is met **
			if(homingProjectileRef->bShouldSpawnProjectile)
			{
				AHomingProjectile::StartHomingRangeAttackFanShape(homingProjectileRef, target
					, homingProjectileRef->homingProjectileClassToSpawn, homingProjectileRef->projectileCount
					, homingProjectileRef->fanRadius,homingProjectileRef->zOffset, homingProjectileRef->projectileSpeed
					, homingProjectileRef->GetActorRotation(), false);

				// Play impact VFX before this get destroyed after new projectiles are spawned
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(homingProjectileRef->GetWorld()
					, homingProjectileRef->impactVFX, homingProjectileRef->GetActorLocation());
							
				homingProjectileRef->Destroy();
				return;
			}
						
			// ** After n seconds, directly make projectile homing toward player **
			homingProjectileRef->stateManager->SwitchStateByKey("HomingLaunching");
		}
	}, homingProjectileRef->GetProjectileMaxPreparingTime(), false);
}

void UHomingPreparingState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
	
}

void UHomingPreparingState::OnExitState()
{
	Super::OnExitState();
}
