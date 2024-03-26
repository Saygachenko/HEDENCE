// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInteractTest.h"

#include "ItemDataComponent.h"
#include "ItemDataStructs.h"
#include "Components/WidgetComponent.h"
//#include "Components/StaticMeshComponent.h"

// Sets default values
AActorInteractTest::AActorInteractTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	SetRootComponent(StaticMeshComponent);

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(FName("ItemData"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
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
FText AActorInteractTest::LookAt()
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