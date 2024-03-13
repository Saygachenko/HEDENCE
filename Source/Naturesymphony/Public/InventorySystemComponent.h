// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataComponent.h"
#include "InventorySystemComponent.generated.h"

// Struct information about a slot in your inventory
USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Content")
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Content")
	int32 Quantity = 0;
};

// Struct for return two variables for slots
USTRUCT(BlueprintType)
struct FInventoryOperationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 Value;

	UPROPERTY(BlueprintReadOnly)
	bool Success;
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

	// Function add to inventory
	UFUNCTION(BlueprintCallable)
	FInventoryOperationResult AddToInventory(FName ItemID, int32 Quantity);

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

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UItemDataComponent* ItemDataComponent = nullptr;

	// Function remove to inventory
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory();

	// Function overlapping with item
	UFUNCTION(BlueprintCallable)
	void InteractionTrace();

	// Function finded slot
	UFUNCTION(BlueprintCallable)
	FInventoryOperationResult FindSlot(FName ItemID);

	// Function get max stack size
	UFUNCTION(BlueprintPure)
	int32 GetMaxStackSize(FName ItemID);

	// Function add to stack item in inventory
	UFUNCTION(BlueprintCallable)
	void AddToStack(int32 Index, int32 Quantity);

	// Function to check all empty slots in the inventory
	UFUNCTION(BlueprintPure)
	FInventoryOperationResult AnyEmptySlotsAvailable();

	// Function to create new stack items
	UFUNCTION(BlueprintCallable)
	bool CreateNewStack(FName ItemID, int32 Quantity);
};
