// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStat.generated.h"

USTRUCT(BlueprintType)
struct FBaseStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BaseValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxValue;
};
