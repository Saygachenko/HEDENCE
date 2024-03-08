// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInteractTest.h"

// Sets default values
AActorInteractTest::AActorInteractTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorInteractTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorInteractTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorInteractTest::LookAt()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting"));
}

