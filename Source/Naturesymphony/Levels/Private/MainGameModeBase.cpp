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
		SaveDataLevelObject = SaveDataLevelClass.GetDefaultObject();
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
					TMultiMap<TSoftClassPtr<AActor>, FTransform>& ActorsAddedMap = SaveDataLevelObject->SaveAddedActors;
					for (const TPair<TSoftClassPtr<AActor>, FTransform>& Pair : ActorsAddedMap)
					{
						TSoftClassPtr<AActor> ActorPtr = Pair.Key;
						FTransform ActorValue = Pair.Value;
						
						UClass* ActorClass = ActorPtr.Get();
						if (ActorClass)
						{
							World->SpawnActor<AActor>(ActorClass, ActorValue);
						}
					}

					TArray<TSoftObjectPtr<AActor>>& ActorsRemovedArray = SaveDataLevelObject->SaveActorsRemoved;
					for (const TSoftObjectPtr<AActor>& Actor : ActorsRemovedArray)
					{
						if (Actor)
						{
							Actor->Destroy();
						}
					}

					UGameplayStatics::SaveGameToSlot(LoadGameDataLevel, GameDataLevel, 0);
				}
			}
			else
			{
				LoadGameDataLevel = UGameplayStatics::CreateSaveGameObject(SaveDataLevelClass);
			}
		}
	}
}

void AMainGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameplayStatics::SaveGameToSlot(LoadGameDataLevel, GameDataLevel, 0);
}