// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataStructs.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase();

public:
	/*UPROPERTY()
	UInventoryComponent* OwningInventory;*/

	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (ClampMin = "1", ClampMax = "32"))
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;

	UFUNCTION()
	UItemBase* CreateItemCopy() const;

	UFUNCTION()
	FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemNumericData.Weight; }

	UFUNCTION()
	FORCEINLINE float GetItemSingleWeight() const { return ItemNumericData.Weight; }

	UFUNCTION()
	FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemNumericData.MaxStackSize; }

	UFUNCTION()
	void SetQuantity(const int32 NewQuantity);

	/*UFUNCTION()
	virtual void Use(ACharacter* Character);*/

protected:
	//bool operator == (const FName& OtherID) const { return ID == OtherID; }
};
