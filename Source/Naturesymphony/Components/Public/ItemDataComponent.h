// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Naturesymphony/Inventory/Public/ItemDataStructs.h"
#include "Naturesymphony/Inventory/Public/InteractInterface.h"
#include "ItemDataComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UItemDataComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDataComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FDataTableRowHandle ItemDataTableRow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data", meta = (ClampMin = 1, ClampMax = 999))
	int32 PickUpQuantity = 1;

	// Function of overriding interaction with interface item
	virtual void InteractWith(ACharacter* PlayerCharacter) override;

	/*void SetPickUpStackSize(int32 NewPickUpStackSize) { PickUpQuantity = NewPickUpStackSize; };
	int32 GetPickUpStackSize() { return PickUpQuantity; };*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};