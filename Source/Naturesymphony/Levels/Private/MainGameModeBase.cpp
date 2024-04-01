// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Levels/Public/MainGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Naturesymphony/SaveGame/Public/SaveDataLevel.h"

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = World->GetName();
		FString GameDataLevel = CurrentLevelName.Append("Data");

		bool DoesSaveGame = UGameplayStatics::DoesSaveGameExist(GameDataLevel, 0);
		if (DoesSaveGame)
		{
			LoadGameDataLevel = UGameplayStatics::LoadGameFromSlot(GameDataLevel, 0);
			if (LoadGameDataLevel)
			{
				USaveDataLevel* SaveDataPlayer = Cast<USaveDataLevel>(LoadGameDataLevel);
				if (SaveDataPlayer)
				{
					UGameplayStatics::SaveGameToSlot(LoadGameDataLevel, GameDataLevel, 0);
				}
			}
		}
		else
		{
			if (SaveDataLevelClass)
			{
				LoadGameDataLevel = UGameplayStatics::CreateSaveGameObject(SaveDataLevelClass);
				if (LoadGameDataLevel)
				{
					UGameplayStatics::SaveGameToSlot(LoadGameDataLevel, GameDataLevel, 0);
				}
			}
		}
	}
}