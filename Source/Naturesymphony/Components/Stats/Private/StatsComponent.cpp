// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Components/Stats/Public/StatsComponent.h"

#include "Naturesymphony/Structs/Stats/Public/BaseStat.h"
#include "Naturesymphony/Enums/Stats/Public/StatsEnum.h"
#include "Naturesymphony/Components/Characters/Public/StateManagerComponent.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::InitializeStats()
{
	for (auto& Pair : BaseStats)
	{
		EStats Stat = Pair.Key;
		FBaseStat* FindKey = BaseStats.Find(Stat);

		SetCurrentStatValue(Stat, FindKey->BaseValue);
	}
}

void UStatsComponent::ModifyCurrentStatValue(EStats Stat, float Value)
{
	if (Value != 0.0f)
	{
		float NewValue = GetCurrentStatValue(Stat) + Value;
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStatValue(Stat));

		SetCurrentStatValue(Stat, NewValue);
	}
}

void UStatsComponent::TakeDamage(float NewDamage)
{
	ModifyCurrentStatValue(EStats::Health, NewDamage * -1.0f);

	if (GetCurrentStatValue(EStats::Health) <= 0.0f)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			UStateManagerComponent* OwnerStateManagerComponent = Owner->GetComponentByClass<UStateManagerComponent>();
			if (OwnerStateManagerComponent)
			{
				OwnerStateManagerComponent->SetState(ECharacterState::Dead);
			}
		}
	}
}