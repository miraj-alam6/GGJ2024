// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Camera->SetupAttachment(Root)

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 80.f);
	//Makes a side view where x axis is forward
	CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; 

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::ApplyThruster(FVector2D Vector)
{
	FVector XZForceDirection = FVector(Vector.X, 0, Vector.Y);
	float ForceScalar = 0.f;
	if (bUseAcceleration) {
		ForceScalar = GetCapsuleComponent()->GetMassScale() * ThrusterAccelertion;
	}
	else {
		ForceScalar = ThrusterForce;
	}

	FVector Force = XZForceDirection * ThrusterForce;

	GetCapsuleComponent()->AddForce(Force);
	if (GEngine) {
		FString Message = FString::Printf(TEXT("%s: Applied Force %s"), *(this->GetName()), *Force.ToString());
		//0 counts as a unique key, -1 means don't use any key
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, Message);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

