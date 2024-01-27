// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomPhysicsActor.generated.h"

UCLASS()
class PROJECTLAUGH_API ACustomPhysicsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomPhysicsActor();
	FVector OffsetWhenAttached;
	void AddConstantForce(const FVector& Force);
	float GetMass();
	FVector GetSimulatedBodyLocation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
