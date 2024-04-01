// Fill out your copyright notice in the Description page of Project Settings.


#include "HFGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Naturesymphony/SaveGame/Public/SaveDataPlayer.h"

// Virtual function to allow custom GameInstances an opportunity to set up what it needs
void UHFGameInstance::Init()
{
	Super::Init();

	bool DoesSaveGame = UGameplayStatics::DoesSaveGameExist(GameDataPlayerSlot, 0);
	if (DoesSaveGame)
	{
		LoadGameDataPlayer = UGameplayStatics::LoadGameFromSlot(GameDataPlayerSlot, 0);
		if (LoadGameDataPlayer)
		{
			USaveDataPlayer* SaveDataPlayer = Cast<USaveDataPlayer>(LoadGameDataPlayer);
			if (SaveDataPlayer)
			{
				UGameplayStatics::SaveGameToSlot(LoadGameDataPlayer, GameDataPlayerSlot, 0);
			}
		}
	}
	else
	{
		if (SaveDataPlayerClass)
		{
			LoadGameDataPlayer = UGameplayStatics::CreateSaveGameObject(SaveDataPlayerClass);
			if (LoadGameDataPlayer)
			{
				UGameplayStatics::SaveGameToSlot(LoadGameDataPlayer, GameDataPlayerSlot, 0);
			}
		}
	}
}