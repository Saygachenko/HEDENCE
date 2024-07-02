// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/GameInstance/Public/HFGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Naturesymphony/SaveGame/Public/SaveDataPlayer.h"

// Virtual function to allow custom GameInstances an opportunity to set up what it needs
void UHFGameInstance::Init()
{
	Super::Init();

	SaveDataPlayerObject = SaveDataPlayerClass.GetDefaultObject();
	if (SaveDataPlayerObject)
	{
		bool DoesSaveGame = UGameplayStatics::DoesSaveGameExist(GameDataPlayerSlot, 0);
		if (DoesSaveGame)
		{
			LoadGameDataPlayer = UGameplayStatics::LoadGameFromSlot(GameDataPlayerSlot, 0);
		}
		else
		{
			LoadGameDataPlayer = UGameplayStatics::CreateSaveGameObject(SaveDataPlayerClass);
		}
	}
}