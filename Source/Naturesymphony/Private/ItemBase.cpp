// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

UItemBase::UItemBase()
{

}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->ItemTextData = this->ItemTextData;
	ItemCopy->ItemNumericData = this->ItemNumericData;
	ItemCopy->ItemAssetData = this->ItemAssetData;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);
		/*if (OwningInventory && Quantity <= 0)
		{
			OwningInventory->RemoveItem(this);
		}*/
	}
}

//void UItemBase::Use(ACharacter* Character)
//{
//
//}
