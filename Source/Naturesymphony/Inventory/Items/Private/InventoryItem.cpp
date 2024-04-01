// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Public/InventoryItem.h"

#include "Naturesymphony/Components/Public/ItemDataComponent.h"

// Sets default values
AInventoryItem::AInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(FName("ItemData"));
}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
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