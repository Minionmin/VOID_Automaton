// Fill out your copyright notice in the Description page of Project Settings.


#include "Buffs/AttributeBuff.h"
#include "Interfaces/HasAttribute.h"

UAttributeBuff::UAttributeBuff()
{
}

void UAttributeBuff::ApplyEffect(AActor* target)
{
	Super::ApplyEffect(target);

	if(attributeComponent == nullptr)
	{
		// オーナーが属性コンポーネントを持っているか確認する
		auto hasAttributeActor = Cast<IHasAttribute>(target);
		if(hasAttributeActor)
		{
			// 属性コンポーネントのレファレンスを取得する
			attributeComponent = hasAttributeActor->GetAttribute_Implementation();	
		}
	}
}

void UAttributeBuff::RemoveEffect(AActor* target)
{
	Super::RemoveEffect(target);
}
