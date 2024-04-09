// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Components/Public/InventorySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Naturesymphony/Inventory/Public/InteractInterface.h"
#include "Naturesymphony/Components/Public/ItemDataComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Naturesymphony/Inventory/Widgets/Public/PickUpMessageWidget.h"
#include "Naturesymphony/Inventory/Items/Effects/Public/ItemEffect.h"
#include "HFGameInstance.h"
#include "Naturesymphony/SaveGame/Public/SaveDataPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Naturesymphony/Levels/Public/MainGameModeBase.h"
#include "Naturesymphony/SaveGame/Public/SaveDataLevel.h"
#include "GameFramework/SaveGame.h"

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

	LoadInventory();
	SlotStructArray.SetNum(InventorySize);

	OnInventoryUpdate.AddDynamic(this, &UInventorySystemComponent::SaveInventory);
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

	OnInventoryUpdate.Broadcast();

	return { QuantityRemaining, !HasFailed };
}

// Function remove to inventory
void UInventorySystemComponent::RemoveFromInventory(int32 IndexSlot, bool RemoveStack, bool IsConsumed)
{
	FName LocalItem = SlotStructArray[IndexSlot].ID;
	int32 LocalQuantity = SlotStructArray[IndexSlot].Quantity;
	if (RemoveStack)
	{
		SlotStructArray[IndexSlot].ID = FName();
		SlotStructArray[IndexSlot].Quantity = 0;

		DropItem(LocalItem, LocalQuantity);
	}
	else
	{
		if (IsConsumed)
		{
			if (SlotStructArray[IndexSlot].Quantity > 1)
			{
				SlotStructArray[IndexSlot].Quantity--;
			}
			else
			{
				SlotStructArray[IndexSlot].ID = FName();
				SlotStructArray[IndexSlot].Quantity = 0;
			}
		}
		else // Delete slot data the item.
		{
			SlotStructArray[IndexSlot].ID = FName();
			SlotStructArray[IndexSlot].Quantity = 0;
		}
	}

	OnInventoryUpdate.Broadcast();
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
			int32 SourceMaxStackSize = GetMaxStackSize(SourceSlot.ID);
			int32 DestinationMaxStackSize = GetMaxStackSize(DestinationSlot.ID);

			// Check for number in the index and maximum stack
			if (SourceSlot.Quantity == SourceMaxStackSize && DestinationSlot.Quantity != DestinationMaxStackSize)
			{
				Swap(SourceSlot, DestinationSlot);
			}
			else if (SourceSlot.Quantity != SourceMaxStackSize && DestinationSlot.Quantity == DestinationMaxStackSize)
			{
				Swap(SourceSlot, DestinationSlot);
			}
			else if (SourceSlot.Quantity == SourceMaxStackSize && DestinationSlot.Quantity == DestinationMaxStackSize)
			{
				Swap(SourceSlot, DestinationSlot);
			}
			else
			{
				int32 TotalQuantity = SourceSlot.Quantity + DestinationSlot.Quantity;

				if (TotalQuantity <= SourceMaxStackSize)
				{
					DestinationSlot.Quantity = TotalQuantity;
					RemoveFromInventory(SourceIndex, false, false);
				}
				else
				{
					int32 RemainingQuantity = TotalQuantity - SourceMaxStackSize;
					SourceSlot.Quantity = RemainingQuantity;
					DestinationSlot.Quantity = SourceMaxStackSize;
				}
			}
		}
		else
		{
			Swap(SourceSlot, DestinationSlot);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Find FSlotStuct check initial Quantity = 0"));
	}

	OnInventoryUpdate.Broadcast();
}

// Function spawn item in world
void UInventorySystemComponent::DropItem(FName ItemID, int32 Quantity)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AMainGameModeBase* GameModeBase = Cast<AMainGameModeBase>(World->GetAuthGameMode());
		if (GameModeBase)
		{
			AActor* InventoryOwner = GetOwner();
			if (InventoryOwner)
			{
				if (Quantity > 0)
				{
					AActor* ItemActorObject = GetItemData(ItemID).ItemClass.GetDefaultObject();
					if (ItemActorObject)
					{
						UClass* ItemActorClass = ItemActorObject->GetClass();
						if (ItemActorClass)
						{
							FVector OwnerLocation = InventoryOwner->GetActorLocation();
							FVector OwnerForwardLocation = InventoryOwner->GetActorForwardVector() * DropLineTraceLength;
							FVector StartLocation = OwnerLocation + OwnerForwardLocation;

							AActor* SpawnedActor = World->SpawnActor<AActor>(ItemActorClass, StartLocation, FRotator::ZeroRotator);
							if (SpawnedActor)
							{
								ItemDataComponent = SpawnedActor->FindComponentByClass<UItemDataComponent>();
								if (ItemDataComponent)
								{
									ItemDataComponent->SetPickUpStackSize(Quantity);

									USaveDataLevel* SaveDataLevel = GameModeBase->SaveDataLevelObject;
									if (SaveDataLevel)
									{
										FTransform SpawnedActorTransform = SpawnedActor->GetActorTransform();
										SaveDataLevel->SaveAddedActors.Add(ItemActorClass, SpawnedActorTransform);

										/*UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(SpawnedActor->GetRootComponent());
										if (MeshComponent)
										{
											MeshComponent->AddImpulse(OwnerForwardLocation * ImpulseVelocity, NAME_None, false);
										}*/
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

// Function overlapping with item
void UInventorySystemComponent::InteractionTrace()
{
	AActor* LastLookedActor = LookAtActor;

	FVector TraceStartEnd = GetOwner()->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// ObjectTypeQuery7 = new object channel name Interactive
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
		if (LookAtActor && LookAtActor != LastLookedActor)
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
						}
					}
				}
			}

			if (LastLookedActor)
			{
				UWidgetComponent* LastWidgetComponent = LastLookedActor->GetComponentByClass<UWidgetComponent>();
				if (LastWidgetComponent)
				{
					UPickUpMessageWidget* LastPickUpMessageWidget = Cast<UPickUpMessageWidget>(LastWidgetComponent->GetUserWidgetObject());
					if (LastPickUpMessageWidget)
					{
						LastPickUpMessageWidget->ShowMessage(FText::GetEmpty());
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
			LookAtActor = nullptr;
		}
	}
}

// Function Interact with item
void UInventorySystemComponent::Interact()
{
	if (LookAtActor)
	{
		ItemDataComponent = LookAtActor->GetComponentByClass<UItemDataComponent>();
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
	if (ItemDataTable)
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, "");
		if (ItemData)
		{
			return *ItemData;
		}
	}

	return FItemData();
}

// Function for button Use item of inventory
void UInventorySystemComponent::ConsumeItem(int32 IndexSlot)
{
	FSlotStruct ConsumeItemIndex = SlotStructArray[IndexSlot];
	FName ConsumeID = ConsumeItemIndex.ID;

	TSubclassOf<AItemEffect> ItemEffectData = GetItemData(ConsumeID).ItemEffect;
	if (ItemEffectData)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();

		AItemEffect* ItemEffect = GetWorld()->SpawnActor<AItemEffect>(ItemEffectData, OwnerLocation, FRotator::ZeroRotator);

		if (ItemEffect)
		{
			RemoveFromInventory(IndexSlot, false, true);
		}
	}

	OnInventoryUpdate.Broadcast();
}

// Function for SaveInventory
void UInventorySystemComponent::SaveInventory()
{
	FString GameDataSlot = GetGameInstance()->GameDataPlayerSlot;

	USaveDataPlayer* SaveDataPlayer = GetGameInstance()->SaveDataPlayerObject;
	if (SaveDataPlayer)
	{
		SaveDataPlayer->SaveSlotStructArray = SlotStructArray;
		UGameplayStatics::SaveGameToSlot(SaveDataPlayer, GameDataSlot, 0);
	}
}

// Function for LoadInventory
void UInventorySystemComponent::LoadInventory()
{
	USaveDataPlayer* SaveDataPlayer = GetGameInstance()->SaveDataPlayerObject;
	if (SaveDataPlayer)
	{
		SlotStructArray = SaveDataPlayer->SaveSlotStructArray;
	}
}

// Function for GetHFGameInstance
UHFGameInstance* UInventorySystemComponent::GetGameInstance() const
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			UHFGameInstance* HFGameInstance = Cast<UHFGameInstance>(GameInstance);
			if (HFGameInstance)
			{
				return HFGameInstance;
			}
		}
	}

	return nullptr;
}

// Function for button Split item of inventory
void UInventorySystemComponent::SplitItem(int32 IndexSlot)
{
	FSlotStruct& SlotArray = SlotStructArray[IndexSlot];
	FName SlotID = SlotArray.ID;
	int32 SlotQuantity = SlotArray.Quantity;

	if (SlotQuantity > 1)
	{
		int32 Divide = FMath::DivideAndRoundUp(SlotQuantity, 2);

		bool NewStackItem = CreateNewStack(SlotID, Divide);
		if (NewStackItem)
		{
			SlotStructArray[IndexSlot].Quantity = SlotQuantity - Divide;
		}
	}

	OnInventoryUpdate.Broadcast();
}