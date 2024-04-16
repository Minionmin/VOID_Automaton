// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchPunchComboState.h"

#include "VOID_Automaton/DebugMacros.h"

void UWitchPunchComboState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);
	
	// Hide when combo has finished
	OnComboEnded.AddDynamic(this, &UWitchPunchComboState::MovePunchNextState);

	// Start attacking
	StartCombo(punchCombo);
}

void UWitchPunchComboState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
}

void UWitchPunchComboState::OnExitState()
{
	Super::OnExitState();
}

void UWitchPunchComboState::MovePunchNextState()
{
	// If successfully hit player, continue attacking
	const auto originalComboEndDelay = comboEndDelay;
	if(bHasHitPlayer) comboEndDelay = 0.1f;
	
	GetWorld()->GetTimerManager().SetTimer(comboEndTimer, [this]()
	{
		if(witchRef->IsDead() || bPlayerDead) return;
		
		if(bHasHitPlayer)
		{
			// Continue attacking if player is hit during the combo
			witchRef->stateManager->SwitchStateByKey("WitchEvade");
		}
		else
		{
			// Idle after finished combo
			witchRef->stateManager->SwitchStateByKey("WitchIdle");
		
		}
	}, comboEndDelay, false);

	comboEndDelay = originalComboEndDelay;
}
