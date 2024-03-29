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
#include "Kismet/GameplayStatics.h"
#include "PhysicsActor.h"


void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!bAdded) {
		bAdded = true;
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
			EnhancedInputComponent->BindAction(ThrusterAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::ApplyThruster);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
			EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Started, this, &ACustomPlayerController::GrapplePress);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
			EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &ACustomPlayerController::GrappleRelease);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
			EnhancedInputComponent->BindAction(RetractAction, ETriggerEvent::Started, this, &ACustomPlayerController::RetractPress);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACustomPlayerController::InteractPress);
		}
		
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		Subsystem->AddMappingContext(GameplayContext, 0);
	}
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	if (PlayerCharacter) 
	{
		FVector MouseWorldPosition;
		FVector MouseWorldDirection;
		DeprojectMousePositionToWorld(MouseWorldPosition, MouseWorldDirection);;
		FVector StartLocation = PlayerCharacter->RopeStartPivot->GetComponentLocation();
		//FVector EndLocation = StartLocation + MouseDirection * 500.f;
		FVector EndLocation = StartLocation + MouseWorldDirection * 1150.f;
		EndLocation.Y = 0;
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.0f, 0, 10.f);
		PlayerCharacter->SetAimEndPointLocation(EndLocation);
	}
}

void ACustomPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	APlayerCharacter* L_PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (L_PlayerCharacter) {
		PlayerCharacter = L_PlayerCharacter;
		//if (GEngine) {
		//	FString Message = FString::Printf(TEXT("%s: Set Player character reference"), *(this->GetName()));
		//	//0 counts as a unique key, -1 means don't use any key
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
		//}
	}
}

void ACustomPlayerController::ApplyThruster(const FInputActionValue& Value)
{


	if (PlayerCharacter) {
		if (PlayerCharacter->IsDead()) {
			return;
		}
		const FVector2D MovementVector = Value.Get<FVector2D>();
		PlayerCharacter->ApplyThruster(MovementVector);
	}
}

void ACustomPlayerController::GrapplePress(const FInputActionValue& Value)
{
	//if (GEngine) {
	//	const FVector2D MovementVector = Value.Get<FVector2D>();
	//	FString Message = FString::Printf(TEXT("%s: Grapple Press"), *(this->GetName()));
	//	//0 counts as a unique key, -1 means don't use any key
	//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);
	//}

	//Old Clicking code
	//ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	//
	//FHitResult HitResult;
	//bool bHit = false;
	//if (LocalPlayer && LocalPlayer->ViewportClient)
	//{
	//	FVector2D MousePosition;
	//	if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
	//	{
	//		bHit = GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, false, HitResult);
	//	}

	//	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.f, 12, FColor::Yellow, false, 5.0f);
	//	AActor* ActorHit = HitResult.GetActor();
	//	if (ActorHit) {
	//		if (GEngine) {
	//			FVector AlteredImpactPoint = HitResult.ImpactPoint;
	//			AlteredImpactPoint.Y = 0;
	//			DrawDebugSphere(GetWorld(), AlteredImpactPoint, 5.f, 12, FColor::Red, false, 5.0f);

	//			//0 counts as a unique key, -1 means don't use any key
	//			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);

	//			if (GEngine) {
	//				const FVector2D MovementVector = Value.Get<FVector2D>();
	//				FString Message = FString::Printf(TEXT("%s: Point %s vs %s. Actor is %s"), *(this->GetName()), *HitResult.ImpactPoint.ToString(), *AlteredImpactPoint.ToString(), *ActorHit->GetName());
	//				//0 counts as a unique key, -1 means don't use any key
	//				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);
	//			}
	//		}
	//	}
	//}

	if (PlayerCharacter) {
		if (PlayerCharacter->IsDead()) {
			return;
		}
		PlayerCharacter->ShootAtAimLocation();
	}

}

void ACustomPlayerController::GrappleRelease(const FInputActionValue& Value)
{
	//if (GEngine) {
	//	const FVector2D MovementVector = Value.Get<FVector2D>();
	//	FString Message = FString::Printf(TEXT("%s: Grapple Release"), *(this->GetName()));
	//	//0 counts as a unique key, -1 means don't use any key
	//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Message);
	//}
}

void ACustomPlayerController::RetractPress(const FInputActionValue& Value)
{
	if (PlayerCharacter) {
		if (PlayerCharacter->IsDead()) {
			return;
		}
		PlayerCharacter->StartRetraction();
	}
}

void ACustomPlayerController::InteractPress(const FInputActionValue& Value)
{
	if (PlayerCharacter) {
		if (PlayerCharacter->IsDead()) {
			return;
		}
		PlayerCharacter->TryToInteract();
	}
}
