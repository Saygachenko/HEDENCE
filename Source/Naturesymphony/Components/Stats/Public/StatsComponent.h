// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Naturesymphony/Structs/Stats/Public/BaseStat.h"
#include "Naturesymphony/Enums/Stats/Public/StatsEnum.h"
#include "StatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentStatValue(EStats Stat, float Value) { CurrentStats.Add(Stat, Value); };

	UFUNCTION(BlueprintPure)
	float GetCurrentStatValue(EStats Stat) { float* Value = CurrentStats.Find(Stat); return Value ? *Value : 0.0f; };

	UFUNCTION(BlueprintPure)
	float GetBaseStatValue(EStats Stat) { FBaseStat* FindKey = BaseStats.Find(Stat); return FindKey->BaseValue; };

	UFUNCTION(BlueprintPure)
	float GetMaxStatValue(EStats Stat) { FBaseStat* FindKey = BaseStats.Find(Stat); return FindKey->MaxValue; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "BaseStats")
	TMap<EStats, FBaseStat> BaseStats;

	TMap<EStats, float> CurrentStats;

	UFUNCTION(BlueprintCallable)
	void InitializeStats();

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentStatValue(EStats Stat, float Value);
		
};