// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInteractTest.h"

#include "ItemDataComponent.h"
#include "ItemDataStructs.h"

// Sets default values
AActorInteractTest::AActorInteractTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	SetRootComponent(SceneComponent);

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(FName("ItemData"));
}

// Called when the game starts or when spawned
void AActorInteractTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorInteractTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function overriding view on item of interface
void AActorInteractTest::LookAt()
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
				UE_LOG(LogTemp, Warning, TEXT("Pick up: %s"), *ItemStruct->Name.ToString());
			}
		}
	}
}