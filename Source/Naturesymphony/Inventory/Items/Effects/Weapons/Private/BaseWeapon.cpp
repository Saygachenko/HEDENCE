// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Effects/Weapons/Public/BaseWeapon.h"

#include "Naturesymphony/Characters/Public/MainPlayerAnimInstance.h"
#include "Naturesymphony/Components/Public/CombatComponent.h"
#include "GameFramework/Character.h"
#include "CollisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

ABaseWeapon::ABaseWeapon()
{
	CollisionComponent = CreateDefaultSubobject<UCollisionComponent>(FName("CollisionComponent"));
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnCollisionHit.AddDynamic(this, &ABaseWeapon::OnHit);
}

void ABaseWeapon::OnHit(const FHitResult& HitResult)
{
	AActor* OwnerWeapon = GetOwner();
	if (OwnerWeapon)
	{
		AActor* DamagedActor = HitResult.GetActor();
		if (DamagedActor)
		{
			AController* InstigatorController = OwnerWeapon->GetInstigatorController();
			if (InstigatorController)
			{
				UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, OwnerWeapon->GetActorForwardVector(), HitResult, InstigatorController, this, UDamageType::StaticClass());
			}
		}
	}
}

// Function for equipping weapons on a the character.
void ABaseWeapon::OnEquipped(ECombatType CombatType)
{
	Super::OnEquipped(CombatType);

	AActor* CharacterOwner = GetOwner();
	if (CharacterOwner)
	{
		UCombatComponent* CombatComponent = CharacterOwner->GetComponentByClass<UCombatComponent>();
		if (CombatComponent)
		{
			ACharacter* OwnerCharacter = Cast<ACharacter>(CharacterOwner);
			if (OwnerCharacter)
			{
				SetIsEuqipped(true);

				if (CombatComponent->GetCombatEnabled())
				{
					AttachActor(HandSocketName);
				}
				else
				{
					AttachActor(AttachSocketName);
				}

				CombatComponent->SetMainWeapon(this);

				USkeletalMeshComponent* OwnerMeshComponent = OwnerCharacter->GetMesh();
				if (OwnerMeshComponent)
				{
					UAnimInstance* OwnerAnimInstance = OwnerMeshComponent->GetAnimInstance();
					if (OwnerAnimInstance)
					{
						UMainPlayerAnimInstance* OwnerMainPlayerAnimInstance = Cast<UMainPlayerAnimInstance>(OwnerAnimInstance);
						if (OwnerMainPlayerAnimInstance)
						{
							OwnerMainPlayerAnimInstance->Execute_UpdateCombatType(OwnerMainPlayerAnimInstance, CombatType);

							if (CollisionComponent)
							{
								CollisionComponent->SetCollisionMesh(GetItemMesh());
								CollisionComponent->AddActorToIgnore(OwnerCharacter);
							}
						}
					}
				}
			}
		}
	}
}