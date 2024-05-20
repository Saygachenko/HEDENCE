// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Effects/Public/EquipEffect.h"

#include "GameFramework/Character.h"

AEquipEffect::AEquipEffect()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	RootComponent = SceneComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
}

void AEquipEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Getter StaticMesh or SkeletalMesh
UPrimitiveComponent* AEquipEffect::GetItemMesh()
{
	UStaticMesh* ItemStaticMesh = StaticMeshComponent->GetStaticMesh();
	if (ItemStaticMesh)
	{
		return StaticMeshComponent;
	}
	else
	{
		return SkeletalMeshComponent;
	}
}

// Basic function of equipping item on a character
void AEquipEffect::OnEquipped(ECombatType CombatType)
{
	bIsEquipped = true;

	AttachActor(AttachSocketName);
}

// Function checks IsEquipped item
void AEquipEffect::OnUnequipped()
{
	if (bIsEquipped)
	{
		bIsEquipped = false;
	}
}

// Function attach actor on a character
void AEquipEffect::AttachActor(FName SocketName)
{
	AActor* OwnerEffects = GetOwner();
	if (OwnerEffects)
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(OwnerEffects);
		if (CharacterOwner)
		{
			USkeletalMeshComponent* CharacterMeshComponent = CharacterOwner->GetMesh();
			if (CharacterMeshComponent)
			{
				this->AttachToComponent(CharacterMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), SocketName);
			}
		}
	}
}