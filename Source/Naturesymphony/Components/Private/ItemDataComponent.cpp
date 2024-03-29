// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Components/Public/ItemDataComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"
#include "Naturesymphony/Components/Public/InventorySystemComponent.h"

// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Function of overriding interaction with interface item
void UItemDataComponent::InteractWith(ACharacter* Character)
{
	if (Character)
	{
		UInventorySystemComponent* InventoryComponent = Character->FindComponentByClass<UInventorySystemComponent>();
		if (InventoryComponent)
		{
			TObjectPtr<const UDataTable> DataTable = ItemDataTableRow.DataTable;
			if (DataTable)
			{
				FName RowName = ItemDataTableRow.RowName;

				FItemData* DataTableRow = DataTable->FindRow<FItemData>(RowName, "");
				if (DataTableRow)
				{
					FName ItemID = DataTableRow->ID;

					auto InventoryResult = InventoryComponent->AddToInventory(ItemID, Quantity);
					if (InventoryResult.Success)
					{
						GetOwner()->Destroy();
					}
				}
			}
		}
	}
}