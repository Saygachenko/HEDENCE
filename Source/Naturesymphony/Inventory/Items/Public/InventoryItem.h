// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Naturesymphony/Interfaces/Inventory/Public/InteractInterface.h"
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
	TObjectPtr<class UItemDataComponent> ItemDataComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UWidgetComponent> WidgetComponent;
};
