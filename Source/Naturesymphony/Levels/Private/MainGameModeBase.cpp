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
		USaveDataLevel* SaveDataLevelObject = SaveDataLevelClass.GetDefaultObject();
		if (SaveDataLevelObject)
		{
			FString CurrentLevelName = World->GetName();
			GameDataLevel = CurrentLevelName.Append("_GameData");

			bool DoesSaveGame = UGameplayStatics::DoesSaveGameExist(GameDataLevel, 0);
			if (DoesSaveGame)
			{
				LoadGameDataLevel = UGameplayStatics::LoadGameFromSlot(GameDataLevel, 0);
				if (LoadGameDataLevel)
				{
					USaveDataLevel* SaveDataLevel = Cast<USaveDataLevel>(LoadGameDataLevel);
					if (SaveDataLevel)
					{
						UGameplayStatics::SaveGameToSlot(LoadGameDataLevel, GameDataLevel, 0);

						TArray<TSoftObjectPtr<AActor>>& ActorsRemovedArray = SaveDataLevelObject->SaveActorsRemoved;
						for (const TSoftObjectPtr<AActor>& Actor : ActorsRemovedArray)
						{
							if (Actor)
							{
								Actor->Destroy();
							}

						}
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
}

void AMainGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameplayStatics::SaveGameToSlot(LoadGameDataLevel, GameDataLevel, 0);
}