// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Naturesymphony/Inventory/Public/InteractInterface.h"
#include "InventoryItem.generated.h"

UCLASS()
class NATURESYMPHONY_API AInventoryItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Function overriding view on item of interface
	UFUNCTION(BlueprintCallable)
	virtual FText LookAt() override;

	// Sets default values for this actor's properties
	AInventoryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the actor destroyed of level
	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UItemDataComponent* ItemDataComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* WidgetComponent = nullptr;
};
