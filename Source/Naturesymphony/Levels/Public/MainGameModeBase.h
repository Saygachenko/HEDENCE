// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

class USaveDataLevel;

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	TSubclassOf<USaveDataLevel> SaveDataLevelClass;

	UPROPERTY()
	USaveDataLevel* SaveDataLevelObject;

private:
	UPROPERTY()
	class USaveGame* LoadGameDataLevel = nullptr;

	// Set name of level in the BeginPlay() function
	FString GameDataLevel = "";
};
