// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "InteractInterface.h"
#include "ItemDataComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "PickUpMessageWidget.h"

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
void UInventorySystemComponent::RemoveFromInventory(int32 IndexSlot, bool RemoveStack /*bool IsConsumed*/)
{
	FName LocalItem = SlotStructArray[IndexSlot].ID;
	int32 LocalQuantity = SlotStructArray[IndexSlot].Quantity;
	if (RemoveStack)
	{
		SlotStructArray[IndexSlot].ID = FName();
		SlotStructArray[IndexSlot].Quantity = 0;

		DropItem(LocalItem, LocalQuantity);

		OnInventoryUpdate.Broadcast();
		/*if (!IsConsumed)
		{
			DropItem(LocalItem, LocalQuantity);
			OnInventoryUpdate.Broadcast();
		}*/
	}
}

// Function finded slot
FInventoryOperationResult UInventorySystemComponent::FindSlot(FName ItemID)
{
	int32 Index = -1;
	bool FoundSlot = false;

	for (int32 i = 0; i < SlotStructArray.Num(); ++i)
	{
		FSlotStruct Slot = SlotStructArray[i];

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
	for (int32 i = 0; i < SlotStructArray.Num(); ++i)
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
		int32 Index = EmptySlotInfo.Value;

		if (SlotStructArray.IsValidIndex(Index))
		{
			SlotStructArray[Index].ID = ItemID;
			SlotStructArray[Index].Quantity = Quantity;

			return true;
		}
	}

	return false;
}

// Function for moving through slots
void UInventorySystemComponent::TrasferSlots(int32 SourceIndex, UInventorySystemComponent* SourceInventory, int32 DestinationIndex)
{
	if (DestinationIndex >= 0)
	{
		FSlotStruct& SourceSlot = SourceInventory->SlotStructArray[SourceIndex];
		FSlotStruct& DestinationSlot = SlotStructArray[DestinationIndex];
		if (SourceSlot.ID == DestinationSlot.ID)
		{
			// Check for number in the index and maximum stack
			if ((SourceSlot.Quantity == GetMaxStackSize(SourceSlot.ID) && DestinationSlot.Quantity != GetMaxStackSize(DestinationSlot.ID)) ||
				(SourceSlot.Quantity != GetMaxStackSize(SourceSlot.ID) && DestinationSlot.Quantity == GetMaxStackSize(DestinationSlot.ID)) ||
				(SourceSlot.Quantity == GetMaxStackSize(SourceSlot.ID) && DestinationSlot.Quantity == GetMaxStackSize(DestinationSlot.ID)))
			{
				Swap(SourceInventory->SlotStructArray[SourceIndex], SlotStructArray[DestinationIndex]);
			}
			else
			{
				int32 TotalQuantity = SourceSlot.Quantity + DestinationSlot.Quantity - GetMaxStackSize(SourceSlot.ID);

				if (TotalQuantity > 0)
				{
					DestinationSlot.Quantity = TotalQuantity;
					DeleteFromInventory(SourceIndex);
				}
				else
				{
					DeleteFromInventory(SourceIndex);
					DestinationSlot.Quantity = TotalQuantity;
				}
			}
		}
		else
		{
			Swap(SourceInventory->SlotStructArray[SourceIndex], SlotStructArray[DestinationIndex]);
		}

		OnInventoryUpdate.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Find FSlotStuct check initial Quantity = 0"));
	}
}

// Function spawn item in world
void UInventorySystemComponent::DropItem(FName ItemID, int32 Quantity)
{
	TSubclassOf<AActor> ItemActorClass = GetItemData(ItemID).ItemClass;
	if (ItemActorClass)
	{
		if (Quantity > 1)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemActorClass, GetDropLocation(), FRotator::ZeroRotator);
			if (SpawnedActor)
			{
				ItemDataComponent = SpawnedActor->FindComponentByClass<UItemDataComponent>();
				if (ItemDataComponent)
				{
					ItemDataComponent->SetStackSize(Quantity);
				}
			}
		}
		else
		{
			GetWorld()->SpawnActor<AActor>(ItemActorClass, GetDropLocation(), FRotator::ZeroRotator);
		}
	}
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
					UWidgetComponent* WidgetComponent = LookAtActor->GetComponentByClass<UWidgetComponent>();
					if (WidgetComponent)
					{
						PickUpMessageWidget = Cast<UPickUpMessageWidget>(WidgetComponent->GetUserWidgetObject());
						if (PickUpMessageWidget)
						{
							PickUpMessageWidget->ShowMessage(ActorInterface->LookAt());

							LastLookedActor = LookAtActor;
							LookAtActor = nullptr;
						}
					}
				}
			}
		}
	}
	else
	{
		if (PickUpMessageWidget)
		{
			PickUpMessageWidget->ShowMessage(FText::GetEmpty());
			PickUpMessageWidget = nullptr;
			LastLookedActor = nullptr;
		}
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

// Function get item data row
FItemData UInventorySystemComponent::GetItemData(FName ItemID)
{
	if (ItemDataComponent)
	{
		TObjectPtr<const UDataTable> DataTable = ItemDataComponent->ItemDataTableRow.DataTable;
		if (DataTable)
		{
			FItemData* ItemData = DataTable->FindRow<FItemData>(ItemID, "");
			if (ItemData)
			{
				return *ItemData;
			}
		}
	}

	return FItemData();
}

// Function item drop location on world
FVector UInventorySystemComponent::GetDropLocation()
{
	FVector Location = GetOwner()->GetActorLocation();
	FVector ForwardLocation = GetOwner()->GetActorForwardVector() * LineTraceLength;
	FVector StartLocation = Location + ForwardLocation;
	FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 500.0f);
	FHitResult HitResult;
	
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 5.0f, 2.0f);
	bool TraceHitResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility);
	if (TraceHitResult)
	{
		return HitResult.Location;
	}

	return FVector::ZeroVector;
}

// Function for button delete item of inventory
void UInventorySystemComponent::DeleteFromInventory(int32 IndexSlot)
{
	SlotStructArray[IndexSlot].ID = FName();
	SlotStructArray[IndexSlot].Quantity = 0;

	OnInventoryUpdate.Broadcast();
}