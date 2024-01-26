// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTLAUGH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	//Parameters
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float ThrusterForce;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	bool bUseAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters")
	float ThrusterAccelertion;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;
private:


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called every frame
	virtual void ApplyThruster(FVector2D Vector);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
