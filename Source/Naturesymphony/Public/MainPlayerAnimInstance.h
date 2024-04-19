// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Naturesymphony/Inventory/Public/AnimInstanceInterface.h"
#include "MainPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NATURESYMPHONY_API UMainPlayerAnimInstance : public UAnimInstance, public IAnimInstanceInterface
{
	GENERATED_BODY()
	
public:
};
