// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HFGameInstance.generated.h"

class USaveDataPlayer;

UCLASS()
class NATURESYMPHONY_API UHFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	FString GameDataPlayerSlot = "GameDataPlayer";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	TSubclassOf<USaveDataPlayer> SaveDataPlayerClass;

	// Virtual function to allow custom GameInstances an opportunity to set up what it needs
	virtual void Init() override;

	UPROPERTY()
	USaveDataPlayer* SaveDataPlayerObject;

private:
	UPROPERTY()
	class USaveGame* LoadGameDataPlayer;
};
