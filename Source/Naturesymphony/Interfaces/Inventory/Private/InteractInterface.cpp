// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Interfaces/Inventory/Public/InteractInterface.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.

// Function overriding view on item
FText IInteractInterface::LookAt()
{
	return FText::GetEmpty();
}

// Function interacting with item
void IInteractInterface::InteractWith(ACharacter* Character)
{

}