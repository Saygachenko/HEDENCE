// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Naturesymphony/Components/Public/ItemDataComponent.h"
#include "InventorySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdateSignature);

// Struct information about a slot in your inventory
USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Content")
	FName ID = FName();

	UPROPERTY(BlueprintReadWrite, Category = "Content")
	int32 Quantity = 0;
};

// Struct for return two variables for slots
USTRUCT(BlueprintType)
struct FInventoryOperationResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;

	UPROPERTY(BlueprintReadOnly)
	bool Success = false;
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

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdateSignature OnInventoryUpdate;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	float DropLineTraceLength = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Invetory Data")
	float ImpulseVelocity = 100.0f;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UItemDataComponent* ItemDataComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemDataTable = nullptr;

	UPROPERTY()
	class UPickUpMessageWidget* PickUpMessageWidget = nullptr;

	// Function remove to inventory
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int32 IndexSlot, bool RemoveStack, bool IsConsumed);

	// Function overlapping with item
	UFUNCTION(BlueprintCallable)
	void InteractionTrace();

	// Function finded slot
	UFUNCTION(BlueprintCallable)
	FInventoryOperationResult FindSlot(FName ItemID);

	// Function get max stack size
	UFUNCTION(BlueprintPure)
	int32 GetMaxStackSize(FName ItemID) { return GetItemData(ItemID).StackSize; };

	// Function add to stack item in inventory
	UFUNCTION(BlueprintCallable)
	void AddToStack(int32 Index, int32 Quantity);

	// Function to check all empty slots in the inventory
	UFUNCTION(BlueprintPure)
	FInventoryOperationResult AnyEmptySlotsAvailable();

	// Function to create new stack items
	UFUNCTION(BlueprintCallable)
	bool CreateNewStack(FName ItemID, int32 Quantity);

	// Function for moving through slots
	UFUNCTION(BlueprintCallable)
	void TrasferSlots(int32 SourceIndex, UInventorySystemComponent* SourceInventory, int32 DestinationIndex);

	// Function spawn item in world
	UFUNCTION(BlueprintCallable)
	void DropItem(FName ItemID, int32 Quantity);

	// Function get item data row
	UFUNCTION(BlueprintPure)
	FItemData GetItemData(FName ItemID);

	// Function for button Use item of inventory
	UFUNCTION(BlueprintCallable)
	void ConsumeItem(int32 IndexSlot);

	// Function for SaveInventory
	UFUNCTION(BlueprintCallable)
	void SaveInventory();

	// Function for LoadInventory
	UFUNCTION(BlueprintCallable)
	void LoadInventory();

	// Function for GetHFGameInstance
	UFUNCTION(BlueprintCallable)
	class UHFGameInstance* GetHFGameInstance() const;

	// Function for button Split item of inventory
	UFUNCTION(BlueprintCallable)
	void SplitItem(int32 IndexSlot);
};