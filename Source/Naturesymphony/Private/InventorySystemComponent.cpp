// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "InteractInterface.h"

// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UInventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionTrace();
}

// Function add to inventory
void UInventorySystemComponent::AddToInventory()
{

}

// Function remove to inventory
void UInventorySystemComponent::RemoveFromInventory()
{

}

// Function overlapping with item
void UInventorySystemComponent::InteractionTrace()
{
	FVector TraceStartEnd = GetOwner()->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	
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
				}
			}
		}
	}
	else
	{
		LastLookedActor = nullptr;
	}
}