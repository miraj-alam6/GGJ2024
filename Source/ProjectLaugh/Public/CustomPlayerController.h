// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputMappingContext;
class APhysicsActor;
class APlayerCharacter;


struct FInputActionValue;

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent();
	virtual void Tick(float DeltaTime) override;
protected:
	//Input
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* GameplayContext;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ThrusterAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* GrappleAction;
	virtual void SetPawn(APawn* InPawn) override;
private:
	APlayerCharacter* PlayerCharacter;
	void ApplyThruster(const FInputActionValue& Value);
	void GrapplePress(const FInputActionValue& Value);
	void GrappleRelease(const FInputActionValue& Value);
};
