// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "ItemDataStructs.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FName ID = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FText Name = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data", meta = (ClampMin = 1, ClampMax = 999))
	int32 StackSize = 1;
};