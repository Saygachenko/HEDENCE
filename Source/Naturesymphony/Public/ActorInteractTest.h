// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "ActorInteractTest.generated.h"

UCLASS()
class NATURESYMPHONY_API AActorInteractTest : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorInteractTest();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function overriding view on item of interface
	UFUNCTION(BlueprintCallable)
	virtual FText LookAt() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UItemDataComponent* ItemDataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* WidgetComponent;
};