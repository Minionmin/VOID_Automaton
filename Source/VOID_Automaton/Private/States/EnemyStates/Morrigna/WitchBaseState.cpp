// Fill out your copyright notice in the Description page of Project Settings.


#include "States/EnemyStates/Morrigna/WitchBaseState.h"

#include "MyCustomUnrealLibrary.h"
#include "VOIDCharacter.h"
#include "Kismet/GameplayStatics.h"

void UWitchBaseState::OnEnterState(AActor* stateOwner)
{
	Super::OnEnterState(stateOwner);

	// Turn enemyRef into witchRef for later
	if(!witchRef)
	{
		witchRef = Cast<AWitch>(enemyRef);
		if(witchRef)
		{
			witchAIController = witchRef->GetAIController();
		}
	}

	// Get playerRef for later
	if(!playerRef)
	{
		// この敵と戦うためにはプレイヤーは常に存在するため、ハード参照には問題ない（プレイヤーが既にレベルにロードされていること）
		playerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if(playerRef)
		{
			VOIDCharacter = Cast<AVOIDCharacter>(playerRef);
			if(VOIDCharacter)
			{
				VOIDCharacter->OnPlayerDead.AddDynamic(this, &UWitchBaseState::OnPlayerDead);
			}
		}
	}
}

void UWitchBaseState::TickState(float deltaTime)
{
	Super::TickState(deltaTime);

	if(witchRef->IsDead() || witchRef->IsScreaming()) return;
	
	// Make enemy always look at player
	UActorUtilities::FaceActor(witchRef, playerRef, deltaTime, facePlayerInterpSpeed);
}

void UWitchBaseState::OnExitState()
{
	Super::OnExitState();
	
}

void UWitchBaseState::StartRandomPattern(FString firstStateKey, FString secondStateKey
                                         , int32 firstStateProbability, FTimerHandle delayHandler, float delay)
{
	// ** Attack Pattern ** //
	// Random which state should switch next
	const int32 pattern = FMath::RandRange(1,100);
	
	// After n second
	GetWorld()->GetTimerManager().SetTimer(delayHandler, [this, pattern, firstStateProbability, firstStateKey, secondStateKey]()
	{
		if(witchRef->IsDead() || bPlayerDead) return;

		if(pattern <= firstStateProbability)
		{
			witchRef->stateManager->SwitchStateByKey(firstStateKey);
		}
		else
		{
			witchRef->stateManager->SwitchStateByKey(secondStateKey);
		}
	}, delay, false);
}

void UWitchBaseState::OnPlayerDead()
{
	// Stop all timers
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// ステートマシンを停止するためのフラグ
	bPlayerDead = true;
	
	// Switch to wait state
	witchRef->stateManager->SwitchStateByKey("WitchWait");
}