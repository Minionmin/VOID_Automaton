 // Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/EnemyBaseState.h"

#include "VOID_Automaton/DebugMacros.h"

 void UEnemyBaseState::OnEnterState(AActor* stateOwner)
 {
  Super::OnEnterState(stateOwner);
  
  // Save enemy ref for later
  if(!enemyRef)
  {
   enemyRef = Cast<AEnemy>(stateOwner);
  }
 }

 void UEnemyBaseState::OnExitState()
 {
  Super::OnExitState();

  
 }
