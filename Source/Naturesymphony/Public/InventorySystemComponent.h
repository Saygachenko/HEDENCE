// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Map.h"
#include "InventorySystemComponent.generated.h"

// Struct information about a slot in your inventory
USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Content")
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Content")
	int32 Quantity;
};

// Struct for return two variables for function FindSlot
USTRUCT(BlueprintType)
struct FSlotResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintRead)
	int32 Index;

	UPROPERTY(BlueprintRead)
	bool FoundSlot;
};

// In the future, the entire component needs to be replicated for online
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function Interact with item
	UFUNCTION()
	void Interact();

protected:
	// We will need to replicate an array of the structure
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	TArray<FSlotStruct> SlotStructArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	int32 InventorySize = 16;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	float SphereTraceRadius = 150.0f;

	UPROPERTY(BlueprintReadWrite)
	AActor* LookAtActor = nullptr;

	UPROPERTY(BlueprintReadWrite)
	AActor* LastLookedActor = nullptr;

	// Function add to inventory
	UFUNCTION(BlueprintCallable)
	void AddToInventory(FName ItemID, int32 Quantity);

	// Function finded slot
	UFUNCTION(BlueprintCallable)
	FSlotResult FindSlot(FName ItemID);

private:
	UPROPERTY()
	UDataTable* ItemsDataTable;

	// Function remove to inventory
	void RemoveFromInventory();

	// Function overlapping with item
	void InteractionTrace();

	// Function get max stack size
	UFUNCTION(BlueprintPure)
	int32 GetMaxStackSize(FName ItemID);
};
