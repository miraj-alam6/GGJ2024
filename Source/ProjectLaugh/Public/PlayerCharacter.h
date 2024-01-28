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
	FullRetracting,
	ShootingOut
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
	void StartRetraction();
	void EndRetraction();
	void UpdateCableEndPoint();
	void TryToInteract();
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

	UFUNCTION(BlueprintCallable)
	void SetFuelToMax();
	UFUNCTION(BlueprintCallable)
	void SetOxygenToMax();
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	UFUNCTION(BlueprintCallable)
	void LogMessage(FText& Text);
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float GrapplePullConstantWithNoDivision;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bDivideByDistance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float GrappleShootOutSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float GrappleFullRetractionSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float GrappleShootOutMaxDistance = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float MaxHealth = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float MaxFuel = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float MaxOxygen = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float NoOxygenHealthLossRate = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float FuelExpenseRate = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float OxygenBaseExpenseRate = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bDebugInfniteFuel = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float OxygenForFuelExpenseRate = 5.f;

	bool bDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACustomPhysicsActor* ObjectToInteractWith;

	float CurrentHealth;
	float CurrentFuel;
	float CurrentOxygen;


	GrappleState CurrentGrappleState = GrappleState::Retracted;
	float CurrentGrappleShootOutDistance;
	FVector CableDirection;

	//Grapple Functions
	bool GetDidCableConnect();
	void GrappleTowardsEachOther();
	void SetCableEndpointToAttachment();

	//Vital Functions
	UFUNCTION(BlueprintCallable)
	void RestoreHealth(float Amount);
	UFUNCTION(BlueprintCallable)
	void LoseHealth(float Amount);
	void ExpendFuel(float Amount);
	void ExpendOxygen(float Amount);


	TArray<FText> LogEntries;

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
	UFUNCTION(BlueprintCallable)
	void AddConstantForce(const FVector& Force);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsCableConnected();

	UFUNCTION(BlueprintCallable)
	float GetFuelPercentage();
	UFUNCTION(BlueprintCallable)
	float GetOxygenPercentage();
	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage();
	UFUNCTION(BlueprintPure)
	bool HasFuel();
	UFUNCTION(BlueprintPure)
	bool HasOxygen();

	UFUNCTION(BlueprintPure)
	bool IsDead();

};
