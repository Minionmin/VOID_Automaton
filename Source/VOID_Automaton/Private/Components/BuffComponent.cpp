// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BuffComponent.h"

#include "Buffs/Buff.h"
#include "Saves/MainGameInstance.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// バフオーナーを設定
	buffOwner = GetOwner();
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

// バフリストにバフを追加し、バフの効果をバフオーナーに適用します
void UBuffComponent::AddBuff(UBuff* buff)
{
	activeBuffs.Add(buff);
}

// バフリストからバフを削除し、バフの効果をバフオーナーから削除します
void UBuffComponent::RemoveBuff(UBuff* buff)
{
	activeBuffs.Remove(buff);
	buff->RemoveEffect(buffOwner);
}

// バフリストに登録されている全てのバフの効果を適用します
void UBuffComponent::ApplyAllBuffEffect()
{
	for(const auto buff : activeBuffs)
	{
		buff->ApplyEffect(buffOwner);
	}
}

void UBuffComponent::RemoveAllBuffs()
{
	for(const auto buff : activeBuffs)
	{
		buff->RemoveEffect(buffOwner);
	}
	activeBuffs.Empty();
}

// バフリストに登録されている全てのバフを取得します（Deep copy）
void UBuffComponent::SetActiveBuffs(const TArray<UBuff*>& buffList)
{
	for(auto buff : buffList)
	{
		activeBuffs.Add(buff);
	}
}

// GameInstanceからバフリストをロードする（バフリストはGameInstanceでゲームファイルから最初にロードされている前提）
void UBuffComponent::LoadActiveBuffs()
{
	if(const auto gameInstance = Cast<UMainGameInstance>(GetWorld()->GetGameInstance()))
	{
		activeBuffs = gameInstance->GetBuffListInstance();
	}

	// バフリストが更新された通知（PlayerInfoUI）
	OnBuffListChanged.Broadcast();
}

// バフコンポネントからバフリストに登録されている全てのバフIDを取得する（データセーブ用）
void UBuffComponent::GetActiveBuffIDs(TArray<int>& buffSaveDataArray) const
{
	for(const auto buff : activeBuffs)
	{
		buffSaveDataArray.Add(buff->GetBuffID());
	}
}

// バフリストをGameInstanceから再読み込みする
void UBuffComponent::ReloadBuffList()
{
	RemoveAllBuffs();
	LoadActiveBuffs();
	ApplyAllBuffEffect();
}

