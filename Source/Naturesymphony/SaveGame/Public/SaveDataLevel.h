// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveDataLevel.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API USaveDataLevel : public USaveGame
{
	GENERATED_BODY()
	
public:
	TArray<AActor> SaveActorsRemoved;
};
