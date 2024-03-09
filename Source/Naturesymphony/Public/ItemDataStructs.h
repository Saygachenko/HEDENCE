// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "ItemDataStructs.generated.h"

// Struct text information about the item
//USTRUCT(BlueprintType)
//struct FItemTextData
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	FText Name;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	FText Description;
//};
//
//USTRUCT(BlueprintType)
//struct FItemNumericData
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	int32 StackSize;
//};
//
//USTRUCT(BlueprintType)
//struct FItemAssetData
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	UTexture2D* Icon;
//};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	int32 StackSize;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FItemTextData ItemTextData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FItemNumericData ItemNumericData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item Data")
	FItemAssetData ItemAssetData;*/
};