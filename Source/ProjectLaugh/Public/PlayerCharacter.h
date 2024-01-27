// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCableComponent;
class ACustomPhysicsActor;

UENUM(BlueprintType)
enum class GrappleState : uint8 {
	Retracted,
	Attached,
	AttachedRetracting,
	FullRetracting
};
 

UCLASS()
class PROJECTLAUGH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	void SetAimEndPointLocation(FVector Location);
	void ShootAtAimLocation();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RopeStartPivot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* AimEndPoint;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCableComponent* Cable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACustomPhysicsActor* AttachedPhysicsActor;
protected:
	bool bIsCableConnected;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	//Parameters
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float ThrusterForce;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bUseAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float ThrusterAccelertion;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float GrapplePullConstant;

	GrappleState CurrentGrappleState = GrappleState::Retracted;

	//Grapple Functions
	bool GetDidCableConnect();
	void GrappleTowardsEachOther();
private:
	FVector AimLocation;
	FVector ShootGoalLocation;
	FVector ShootCurrentLocation;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called every frame
	virtual void ApplyThruster(FVector2D Vector);

	float GetMass();
	void AddConstantForce(const FVector& Force);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsCableConnected();



};