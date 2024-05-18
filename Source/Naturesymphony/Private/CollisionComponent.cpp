// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionComponent.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UCollisionComponent::UCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsCollisionEnabled)
    {
        CollisionTrace();
    }
}

// Function enabled trace collision
void UCollisionComponent::EnableCollision()
{
    ClearHitActors();
    bIsCollisionEnabled = true;
}

// Function disabled trace collision
void UCollisionComponent::DisableCollision()
{
    bIsCollisionEnabled = false;
}

// Function for sphere collision trace operation
void UCollisionComponent::CollisionTrace()
{
    FVector TraceStart = CollisionMesh->GetSocketLocation(StartSocketName);
    FVector TraceEnd = CollisionMesh->GetSocketLocation(EndSocketName);

    TArray<FHitResult> HitResults;

    bool SphereTraceResult = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        TraceStart,
        TraceEnd,
        TraceRadius,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        HitResults,
        true);
    if (SphereTraceResult)
    {
        for (const FHitResult& HitResult : HitResults)
        {
            if (HitResult.GetActor() && !AlreadyHitActors.Contains(HitResult.GetActor()))
            {
                AlreadyHitActors.Add(HitResult.GetActor());
                OnCollisionHit.Broadcast(HitResult);
            }
        }
    }
}

// Function for cleaning actors from the array
void UCollisionComponent::ClearHitActors()
{
	if (!AlreadyHitActors.IsEmpty())
	{
		AlreadyHitActors.Empty();
	}
}