// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "CableComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CustomPhysicsActor.h"


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

	RopeStartPivot = CreateDefaultSubobject<USceneComponent>(TEXT("RopeStartPivot"));
	RopeStartPivot->SetupAttachment(GetRootComponent());
	
	AimEndPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimEndPoint"));
	AimEndPoint->SetupAttachment(GetRootComponent());

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(RopeStartPivot);
}

void APlayerCharacter::SetAimEndPointLocation(FVector Location)
{
	AimEndPoint->SetWorldLocation(Location);
	AimLocation = Location;
}

void APlayerCharacter::ShootAtAimLocation()
{
	if (CurrentGrappleState == GrappleState::Retracted) {
		ShootGoalLocation = AimLocation;
		ShootCurrentLocation = AimLocation;
		UpdateCableEndPoint();

		//APhysicsActor* OutPhysicsActor;
		if (GetDidCableConnect()) {
			AttachedPhysicsActor->OffsetWhenAttached = ShootCurrentLocation - AttachedPhysicsActor->GetSimulatedBodyLocation();
			CurrentGrappleState = GrappleState::AttachedRetracting;
		}
	}
}

void APlayerCharacter::UpdateCableEndPoint()
{

	FVector CorrectEndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), ShootCurrentLocation);
	Cable->EndLocation = CorrectEndLocation;
	if (GEngine) {
		FString Message = FString::Printf(TEXT("%s: Stuff World %s  Local Cable %s"), *(this->GetName()), *ShootCurrentLocation.ToString(), *CorrectEndLocation.ToString());
		//0 counts as a unique key, -1 means don't use any key
		GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Green, Message);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Cable->EndLocation = FVector::ZeroVector;
	
}

bool APlayerCharacter::GetDidCableConnect()
{
	float SphereSize = 30.f;
	TArray<AActor*> OutActors;
	TArray<AActor*> IgnoreActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(this, ShootCurrentLocation, SphereSize, TraceObjectTypes, ACustomPhysicsActor::StaticClass(), IgnoreActors, OutActors);
	DrawDebugSphere(GetWorld(), ShootCurrentLocation, SphereSize, 12, FColor::Yellow, false, 0.1);

	if (bHit) {		
		for (AActor* Actor : OutActors) {
			ACustomPhysicsActor* PhysicsActor = Cast<ACustomPhysicsActor>(Actor);
			if (PhysicsActor) {				
				AttachedPhysicsActor = PhysicsActor;
				return true;
			}
		}
	}
		//Grapple Functions

	return false;

}

void APlayerCharacter::GrappleTowardsEachOther()
{
	if (GEngine) {
		FString Message = FString::Printf(TEXT("%s: Marker 1"), *(this->GetName()));
		//0 counts as a unique key, -1 means don't use any key
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, Message);
	}
	if (AttachedPhysicsActor) {
		float ForceScalar;

		//GetActorLocation is for player because this is player. 
		FVector ActorToPlayerDisplacement = (GetActorLocation() - AttachedPhysicsActor->GetSimulatedBodyLocation());
		float DistanceSquared = ActorToPlayerDisplacement.SquaredLength();
		FVector ActorToPlayerDirection = (ActorToPlayerDisplacement).GetSafeNormal();
		FVector PlayerToActorDirection = ActorToPlayerDirection * -1.f;

		//Just using gravity formula as an approximation of grapple hook physics, but removing distance from equation may feel better.
		if (bDivideByDistance) {
			ForceScalar = (GrapplePullConstant * AttachedPhysicsActor->GetMass() * GetMass()) / DistanceSquared;
		}
		else {
			ForceScalar = (GrapplePullConstantWithNoDivision * AttachedPhysicsActor->GetMass() * GetMass());
		}

		FVector ForceUponPlayer = PlayerToActorDirection * ForceScalar;
		FVector ForceUponActor = ActorToPlayerDirection * ForceScalar;


		AttachedPhysicsActor->AddConstantForce(ForceUponActor);
		AddConstantForce(ForceUponPlayer);

		//if (GEngine) {
		//	FString Message = FString::Printf(TEXT("%s: All parts of scalar %f : %f , %f, %f, %f, "), *(this->GetName()), 
		//		ForceScalar, GrapplePullConstant, AttachedPhysicsActor->GetMass(), GetMass(), DistanceSquared);
		//	//0 counts as a unique key, -1 means don't use any key
		//	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green, Message);
		//}
		//if (GEngine) {
		//	FString Message = FString::Printf(TEXT("%s: Applied Force on Actor %s"), *(this->GetName()), *ForceUponActor.ToString());
		//	//0 counts as a unique key, -1 means don't use any key
		//	GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Green, Message);
		//}
		//if (GEngine) {
		//	FString Message = FString::Printf(TEXT("%s: Applied Force on Player %s"), *(this->GetName()), *ForceUponPlayer.ToString());
		//	//0 counts as a unique key, -1 means don't use any key
		//	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Green, Message);
		//}

	}
}

void APlayerCharacter::SetCableEndpointToAttachment()
{
	if (AttachedPhysicsActor) {
		ShootCurrentLocation = AttachedPhysicsActor->GetSimulatedBodyLocation();
		UpdateCableEndPoint();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentGrappleState){
		case GrappleState::Retracted:

		break;
		case GrappleState::Attached:

			break;
		case GrappleState::AttachedRetracting:
			GrappleTowardsEachOther();
			SetCableEndpointToAttachment();
			break;
		case GrappleState::FullRetracting:

			break;
	}
}

void APlayerCharacter::ApplyThruster(FVector2D Vector)
{
	FVector XZForceDirection = FVector(Vector.X, 0, Vector.Y);
	float ForceScalar = 0.f;
	if (bUseAcceleration) {
		ForceScalar = GetMass() * ThrusterAccelertion;
	}
	else {
		ForceScalar = ThrusterForce;
	}

	FVector Force = XZForceDirection * ForceScalar;

	AddConstantForce(Force);
	//if (GEngine) {
	//	FString Message = FString::Printf(TEXT("%s: Applied Force %s"), *(this->GetName()), *Force.ToString());
	//	//0 counts as a unique key, -1 means don't use any key
	//	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, Message);
	//}
}

float APlayerCharacter::GetMass()
{
	return GetCapsuleComponent()->GetMassScale();
}

void APlayerCharacter::AddConstantForce(const FVector& Force)
{
	GetCapsuleComponent()->AddForce(Force);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool APlayerCharacter::GetIsCableConnected()
{
	return bIsCableConnected;
}

