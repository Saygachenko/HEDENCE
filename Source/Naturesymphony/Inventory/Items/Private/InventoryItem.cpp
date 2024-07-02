// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Public/InventoryItem.h"

#include "Naturesymphony/Components/Inventory/Public/ItemDataComponent.h"
#include "Naturesymphony/Levels/Public/MainGameModeBase.h"
#include "Naturesymphony/SaveGame/Public/SaveDataLevel.h"
#include "Components/WidgetComponent.h"

// Sets default values
AInventoryItem::AInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	RootComponent = SceneComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("WidgetComponent"));
	WidgetComponent->SetupAttachment(SkeletalMeshComponent);

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(FName("ItemData"));
}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the actor destroyed of level
void AInventoryItem::Destroyed()
{
	Super::Destroyed();

	UWorld* World = GetWorld();
	if (World)
	{
		AMainGameModeBase* GameModeBase = Cast<AMainGameModeBase>(World->GetAuthGameMode());
		if (GameModeBase)
		{
			USaveDataLevel* SaveDataLevel = GameModeBase->SaveDataLevelObject;
			if (SaveDataLevel)
			{
				SaveDataLevel->SaveActorsRemoved.AddUnique(this);
			}
		}
	}
}

FText AInventoryItem::LookAt()
{
	if (ItemDataComponent)
	{
		FName RowName = ItemDataComponent->ItemDataTableRow.RowName;

		TObjectPtr<const UDataTable> DataTable = ItemDataComponent->ItemDataTableRow.DataTable;
		if (DataTable)
		{
			FItemData* ItemStruct = DataTable->FindRow<FItemData>(RowName, "No appropriate row name!");
			if (ItemStruct)
			{
				FText Message = FText::FromString("E Pick up");

				return Message;
			}
		}
	}

	return FText::GetEmpty();
}