// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "InteractInterface.h"
#include "ItemDataComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	SlotStructArray.SetNum(InventorySize);
}

// Called every frame
void UInventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionTrace();
}

// Function add to inventory
FInventoryOperationResult UInventorySystemComponent::AddToInventory(FName ItemID, int32 Quantity)
{
	bool HasFailed = false;
	int32 QuantityRemaining = Quantity;

	while (QuantityRemaining > 0 && !HasFailed)
	{
		if (FindSlot(ItemID).Success)
		{
			int32 IndexItemSlot = FindSlot(ItemID).Value;
			AddToStack(IndexItemSlot, 1);
			QuantityRemaining--;
		}
		else
		{
			if (AnyEmptySlotsAvailable().Success)
			{
				if (CreateNewStack(ItemID, 1))
				{
					QuantityRemaining--;
				}
				else
				{
					HasFailed = true;
				}
			}
			else
			{
				HasFailed = true;
			}
		}
	}

	return { QuantityRemaining, !HasFailed };
}

// Function remove to inventory
void UInventorySystemComponent::RemoveFromInventory()
{

}

// Function finded slot
FInventoryOperationResult UInventorySystemComponent::FindSlot(FName ItemID)
{
	int32 Index = -1;
	bool FoundSlot = false;

	for (int32 i = 0; i < SlotStructArray.Num(); ++i)
	{
		const auto& Slot = SlotStructArray[i];

		FName SlotID = Slot.ID;
		int32 SlotQuantity = Slot.Quantity;

		if (SlotID == ItemID && SlotQuantity < GetMaxStackSize(ItemID))
		{
			Index = i;
			FoundSlot = true;

			break;
		}
	}

	return { Index, FoundSlot };
}

// Function get max stack size
int32 UInventorySystemComponent::GetMaxStackSize(FName ItemID)
{
	FName RowName = ItemID;
	
	TObjectPtr<const UDataTable> DataTable = ItemDataComponent->ItemDataTableRow.DataTable;
	if (DataTable)
	{
		FItemData* ItemData = DataTable->FindRow<FItemData>(RowName, "");
		if (ItemData)
		{
			int32 StackSize = ItemData->StackSize;

			return StackSize;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Item data not found for ItemID: %s"), *ItemID.ToString());
	}

	return 0;
}

// Function add to stack item in inventory
void UInventorySystemComponent::AddToStack(int32 Index, int32 Quantity)
{
	if (SlotStructArray.IsValidIndex(Index))
	{
		SlotStructArray[Index].Quantity += Quantity;
	}
}

// Function to check all empty slots in the inventory
FInventoryOperationResult UInventorySystemComponent::AnyEmptySlotsAvailable()
{
	for (int i = 0; i < SlotStructArray.Num(); ++i)
	{
		const auto& Slot = SlotStructArray[i];
		if (Slot.Quantity == 0)
		{
			return { i, true };
		}
	}

	return { -1, false };
}

// Function to create new stack items
bool UInventorySystemComponent::CreateNewStack(FName ItemID, int32 Quantity)
{
	FInventoryOperationResult EmptySlotInfo = AnyEmptySlotsAvailable();

	if (EmptySlotInfo.Success)
	{
		const int32& Index = EmptySlotInfo.Value;

		if (SlotStructArray.IsValidIndex(Index))
		{
			SlotStructArray[Index].ID = ItemID;
			SlotStructArray[Index].Quantity = Quantity;

			return true;
		}
	}

	return false;
}

// Function overlapping with item
void UInventorySystemComponent::InteractionTrace()
{
	FVector TraceStartEnd = GetOwner()->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult HitResult;

	bool SphereTraceResult = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), 
		TraceStartEnd, 
		TraceStartEnd, 
		SphereTraceRadius,
		ObjectTypes,
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::ForOneFrame, 
		HitResult, 
		true);
	if (SphereTraceResult)
	{
		LookAtActor = HitResult.GetActor();
		if (LookAtActor != LastLookedActor)
		{
			bool InterfaceResult = LookAtActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
			if (InterfaceResult)
			{
				IInteractInterface* ActorInterface = Cast<IInteractInterface>(LookAtActor);
				if (ActorInterface)
				{
					ActorInterface->LookAt();
					LastLookedActor = LookAtActor;
					LookAtActor = nullptr;
				}
			}
		}
	}
	else
	{
		LastLookedActor = nullptr;
	}
}

// Function Interact with item
void UInventorySystemComponent::Interact()
{
	if (LastLookedActor)
	{
		ItemDataComponent = LastLookedActor->GetComponentByClass<UItemDataComponent>();
		if (ItemDataComponent)
		{
			IInteractInterface* ComponentInterface = Cast<IInteractInterface>(ItemDataComponent);
			if (ComponentInterface)
			{
				ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());
				if (PlayerCharacter)
				{
					ComponentInterface->InteractWith(PlayerCharacter);
				}
			}
		}
	}
}