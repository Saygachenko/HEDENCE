// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveDataLevel.generated.h"

USTRUCT()
struct FSaveItemData
{
	GENERATED_BODY()

	//FTransform Transform = FTransform();

	//int32 PickUpStackSize = 1;
};

UCLASS()
class NATURESYMPHONY_API USaveDataLevel : public USaveGame
{
	GENERATED_BODY()
	
public:
	//TArray<TSoftObjectPtr<AActor>> SaveActorsRemoved;

	//TMultiMap<TSoftClassPtr<AActor>, FSaveItemData> SaveAddedActors;
};
