// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataComponent.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set defaults DataTable
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/Naturesymphony/Inventory/ItemsData/DT_Items"));
	if (DataTableAsset.Succeeded())
	{
		ItemID.DataTable = DataTableAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable asset not found!"));
	}
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
void UItemDataComponent::InteractWith()
{
	GetOwner()->Destroy();
}