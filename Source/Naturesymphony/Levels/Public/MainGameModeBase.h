// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

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
	TSubclassOf<class USaveDataLevel> SaveDataLevelClass;

private:
	UPROPERTY()
	class USaveGame* LoadGameDataLevel;

	// Set name of level in the BeginPlay() function
	FString GameDataLevel = "";
};
