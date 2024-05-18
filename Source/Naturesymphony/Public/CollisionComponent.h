// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollisionHitSignature, const FHitResult&, HitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FOnCollisionHitSignature OnCollisionHit;

	// Sets default values for this component's properties
	UCollisionComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetAlreadyActor() { return AlreadyHitActors; };

	UFUNCTION(BlueprintPure)
	bool GetIsCollisionEnabled() { return bIsCollisionEnabled; };

	UFUNCTION(BlueprintCallable)
	void SetIsCollisionEnabled(bool bCollision) { bIsCollisionEnabled = bCollision; };

	UFUNCTION(BlueprintPure)
	UPrimitiveComponent* GetCollisionMesh() { return CollisionMesh; };

	UFUNCTION(BlueprintCallable)
	void SetCollisionMesh(UPrimitiveComponent* MeshComp) { CollisionMesh = MeshComp; };

	UFUNCTION(BlueprintCallable)
	void AddActorToIgnore(AActor* ActorIgnore) { ActorsToIgnore.AddUnique(ActorIgnore); };

	UFUNCTION(BlueprintCallable)
	void RemoveActorToIgnore(AActor* RemoveActorIgnore) { ActorsToIgnore.Remove(RemoveActorIgnore); };

	TArray<AActor*> GetActorToIgnore() { return ActorsToIgnore; };

protected:
	UPROPERTY(EditDefaultsOnly)
	FName StartSocketName = FName();

	UPROPERTY(EditDefaultsOnly)
	FName EndSocketName = FName();

	UPROPERTY(EditDefaultsOnly)
	float TraceRadius = 20.0f;

	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	bool bIsCollisionEnabled = false;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	UPrimitiveComponent* CollisionMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	// Function enabled trace collision
	UFUNCTION(BlueprintCallable)
	void EnableCollision();

	// Function disabled trace collision
	UFUNCTION(BlueprintCallable)
	void DisableCollision();
	
	// Function for sphere collision trace operation
	void CollisionTrace();

	// Function for cleaning actors from the array
	void ClearHitActors();
};