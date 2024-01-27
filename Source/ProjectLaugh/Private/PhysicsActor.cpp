// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsActor.h"
#include "Components/StaticMeshComponent.h"
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead
//DO NOT USE THIS, PROJECT IS BUGGY AND ANY BLUEPRINT CREATED WITH THIS CLASS AUTOMATICALLY BECOMES A STATIC MESH ACTOR IF YOU CLOSE AND REOOPEN UNREAL
//USE CustomPhysicsActor.h instead

// Sets default values
APhysicsActor::APhysicsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	//GetActorTransform()->SetMobi
	//StaticMesh->SetSimulatePhysics(true);
	//StaticMesh->SetEnableGravity(false);
	FBodyInstance* BodyInstance = StaticMesh->GetBodyInstance();
	if (BodyInstance) {
		BodyInstance->bLockYTranslation = true;
		BodyInstance->bLockXRotation = true;
		BodyInstance->bLockZRotation = true;
		BodyInstance->SetDOFLock(EDOFMode::SixDOF);
	}
	
}

// Called when the game starts or when spawned
void APhysicsActor::BeginPlay()
{
	Super::BeginPlay();
	//StaticMesh->SetSimulatePhysics(true);
	//StaticMesh->SetEnableGravity(false);
	//FBodyInstance* BodyInstance = StaticMesh->GetBodyInstance();
	//if (BodyInstance) {
	//	BodyInstance->bLockYTranslation = true;
	//	BodyInstance->bLockXRotation = true;
	//	BodyInstance->bLockZRotation = true;
	//	BodyInstance->SetDOFLock(EDOFMode::SixDOF);
	//}
}

// Called every frame
void APhysicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

