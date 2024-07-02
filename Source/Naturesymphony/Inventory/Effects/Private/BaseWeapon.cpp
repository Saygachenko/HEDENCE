// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Effects/Public/BaseWeapon.h"

#include "Naturesymphony/Characters/Public/MainPlayerAnimInstance.h"
#include "Naturesymphony/Components/Combat/Public/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Naturesymphony/Components/Combat/Public/CollisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Naturesymphony/Enums/Combat/Public/CharacterAction.h"

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

TArray<UAnimMontage*> ABaseWeapon::GetActionMontages(ECharacterAction CharacterAction)
{
	TArray<UAnimMontage*> CharacterActionMontage;

	switch (CharacterAction)
	{
	case ECharacterAction::None:

		break;

	case ECharacterAction::GeneralAction:

		break;

	case ECharacterAction::LightAttack:

		CharacterActionMontage = LightAttackMontageArray;
		break;

	case ECharacterAction::EnterCombat:

		CharacterActionMontage.Add(EnterCombat);
		break;

	case ECharacterAction::ExitCombat:

		CharacterActionMontage.Add(ExitCombat);
		break;

	default:

		break;
	}

	return CharacterActionMontage;
}
