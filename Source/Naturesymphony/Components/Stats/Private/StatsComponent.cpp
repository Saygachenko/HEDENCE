// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Components/Stats/Public/StatsComponent.h"

#include "Naturesymphony/Structs/Stats/Public/BaseStat.h"
#include "Naturesymphony/Enums/Stats/Public/StatsEnum.h"

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

	InitializeStats();
}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::InitializeStats()
{
	for (auto Pair : BaseStats)
	{
		EStats Stat = Pair.Key;
		FBaseStat* FindKey = BaseStats.Find(Stat);
		float Value = FindKey->BaseValue;

		SetCurrentStatValue(Stat, Value);
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