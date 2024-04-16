// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchKickComboState.h"

void UWitchKickComboState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);
	
	// When combo has finished
	OnComboEnded.AddDynamic(this, &UWitchKickComboState::MoveKickNextState);

	if(witchRef->bhasEnteredSecondPhase)
	{
		// Start 2nd phase kick combo
		StartCombo(secondKickCombo);
	}
	else
	{
		// Start normal kick combo
		StartCombo(kickCombo);
	}

}

void UWitchKickComboState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);
}

void UWitchKickComboState::OnExitState()
{
	Super::OnExitState();

	// Clear all delegates
	OnComboEnded.Clear();
}

void UWitchKickComboState::MoveKickNextState()
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
