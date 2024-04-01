// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Public/BaseWeapon.h"

#include "Components/WidgetComponent.h"

ABaseWeapon::ABaseWeapon()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	SetRootComponent(SceneComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("WidgetComponent"));
	WidgetComponent->SetupAttachment(SkeletalMeshComponent);

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
}
