// Fill out your copyright notice in the Description page of Project Settings.


#include "HFGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Naturesymphony/SaveGame/Public/SaveDataPlayer.h"

// Virtual function to allow custom GameInstances an opportunity to set up what it needs
void UHFGameInstance::Init()
{
	Super::Init();

	bool DoesSaveGame = UGameplayStatics::DoesSaveGameExist(GameDataSlot, 0);
	if (DoesSaveGame)
	{
		LoadGameData = UGameplayStatics::LoadGameFromSlot(GameDataSlot, 0);
		if (LoadGameData)
		{
			USaveDataPlayer* SaveDataPlayer = Cast<USaveDataPlayer>(LoadGameData);
			if (SaveDataPlayer)
			{
				UGameplayStatics::SaveGameToSlot(LoadGameData, GameDataSlot, 0);
			}
		}
	}
	else
	{
		if (SaveDataPlayerClass)
		{
			LoadGameData = UGameplayStatics::CreateSaveGameObject(SaveDataPlayerClass);
			if (LoadGameData)
			{
				UGameplayStatics::SaveGameToSlot(LoadGameData, GameDataSlot, 0);
			}
		}
	}
}