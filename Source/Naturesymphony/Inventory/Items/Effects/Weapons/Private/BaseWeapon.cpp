// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Effects/Weapons/Public/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;
}