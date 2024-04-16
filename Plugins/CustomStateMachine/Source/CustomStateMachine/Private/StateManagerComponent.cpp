// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManagerComponent.h"


UStateManagerComponent::UStateManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ** Create State and hold them in memory(stateMap) for when needed **
	InitializeState();
}

void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if(!bCanTickState) return;
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(currentState->bCanTickState)
	{
		currentState->TickState(DeltaTime);
	}

}

void UStateManagerComponent::InitStateManager()
{
	SwitchStateByKey(initialState);
}

void UStateManagerComponent::InitializeState()
{
	// ** ERROR HANDLING 1: If there is no available state ** 
	if(availableStates.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, this->GetOwner()->GetName()
			+ "has no state available");	
		return;
	}
	
	// ** Create State and hold them in memory for when needed **
	for(auto it = availableStates.CreateConstIterator(); it; ++it)
	{
		// ** ERROR HANDLING 2: User forgot to assign BP_(State) **
		if(!it->Value)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, it->Key
				+ " is not assigned");	
			return;
		}
		
		UStateBase* state = NewObject<UStateBase>(this, it->Value);
		stateMap.Add(it->Key, state);
	}
}

void UStateManagerComponent::SwitchStateByKey(FString stateKey)
{
	// ** Bind the state (by key) **
	UStateBase* newState = stateMap.FindRef(stateKey);

	// ** ERROR HANDLING: No state found by assigned key **
	if(!newState)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "There is no key name: "
			+ stateKey + " in " + GetOwner()->GetName());	
		return;
	}
	
	SwitchState(newState);
}

void UStateManagerComponent::SwitchState(UStateBase* newState)
{
	// ** Nullptr check for new state **
	if(newState->IsValidLowLevel())
	{
		// ** If there is no current state, it means we are at init **
		if(!currentState)
		{
			currentState = newState;
		}
		else
		{
			// ** If current state is the same class with new state, but is not repeatable, we error debug log **
			if(currentState->GetClass() == newState->GetClass() && currentState->bCanRepeat == false)
			{
				if(bDebug)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, this->GetOwner()->GetName()
						+ "'s state switch failed. State is repeated. " + currentState->stateDisplayName.GetPlainNameString());	
				}
			}
			// ** If current state is the same class with new state, and is repeatable 
			// or If new state is not the same with current state
			else
			{
				// ** Disable tick function and call state exit function
				bCanTickState = false;

				currentState->OnExitState();
				
				// ** If state history is not full, we just push current state into the state history
				if(stateHistoryLength > 0 && stateHistory.Num() < stateHistoryLength)
				{
					stateHistory.Push(currentState);
				}
				// ** If state history is full, we clear the oldest history first then push current state into the state history
				else
				{
					if(stateHistoryLength > 0)
					{
						stateHistory.RemoveAt(0);
						stateHistory.Push(currentState);
					}
				}
				
				if(bDebug && bShowStateProgress)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange,"From "
						+ currentState->stateDisplayName.GetPlainNameString() + " to " + newState->stateDisplayName.GetPlainNameString());	
				}
				
				currentState = newState;
			}
		}

		// ** After checking for all condition and state has changed **
		if(currentState)
		{
			currentState->OnEnterState(GetOwner());
			bCanTickState = true;
		}

		// ** DEBUG ** //
		if(bDebug)
		{
			if(currentState)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, this->GetOwner()->GetName()
					+ "'s current state: " + currentState->stateDisplayName.GetPlainNameString());
			}
			if(stateHistory.Num() > 0)
			{
				for(int32 i = 0; i < stateHistory.Num(); i++)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, this->GetOwner()->GetName()
						+ "'s past state: " + FString::FromInt(i+1) + " " + stateHistory[i]->GetName());	
				}
			}
		}
	}
	// ** If new state is null **
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, this->GetOwner()->GetName()
			+ "'s state switch failed." + currentState->stateDisplayName.GetPlainNameString());	
	}
}