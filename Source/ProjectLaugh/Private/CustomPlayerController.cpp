// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.h"


void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(ThrusterAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::ApplyThruster);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Started, this, &ACustomPlayerController::GrapplePress);
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &ACustomPlayerController::GrappleRelease);
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(GameplayContext, 0);
}

void ACustomPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	APlayerCharacter* L_PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (L_PlayerCharacter) {
		PlayerCharacter = L_PlayerCharacter;
		if (GEngine) {
			FString Message = FString::Printf(TEXT("%s: Set Player character reference"), *(this->GetName()));
			//0 counts as a unique key, -1 means don't use any key
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
		}
	}
}

void ACustomPlayerController::ApplyThruster(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (PlayerCharacter) {
		PlayerCharacter->ApplyThruster(MovementVector);
	}
}

void ACustomPlayerController::GrapplePress(const FInputActionValue& Value)
{
	if (GEngine) {
		const FVector2D MovementVector = Value.Get<FVector2D>();
		FString Message = FString::Printf(TEXT("%s: Grapple Press"), *(this->GetName()));
		//0 counts as a unique key, -1 means don't use any key
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);
	}
}

void ACustomPlayerController::GrappleRelease(const FInputActionValue& Value)
{
	if (GEngine) {
		const FVector2D MovementVector = Value.Get<FVector2D>();
		FString Message = FString::Printf(TEXT("%s: Grapple Release"), *(this->GetName()));
		//0 counts as a unique key, -1 means don't use any key
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);
	}
}
