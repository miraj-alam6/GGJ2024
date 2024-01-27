// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPhysicsActor.h"

// Sets default values
ACustomPhysicsActor::ACustomPhysicsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	FBodyInstance* BodyInstance = StaticMesh->GetBodyInstance();
	if (BodyInstance) {
		BodyInstance->bLockYTranslation = true;
		BodyInstance->bLockXRotation = true;
		BodyInstance->bLockZRotation = true;
		BodyInstance->SetDOFLock(EDOFMode::SixDOF);
	}
}

void ACustomPhysicsActor::AddConstantForce(const FVector& Force)
{
	StaticMesh->AddForce(Force);
}

float ACustomPhysicsActor::GetMass()
{
	return StaticMesh->GetMassScale();
}

// Called when the game starts or when spawned
void ACustomPhysicsActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomPhysicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
