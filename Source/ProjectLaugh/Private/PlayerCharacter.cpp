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
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"


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
	if (CurrentGrappleState == GrappleState::Retracted || CurrentGrappleState == GrappleState::FullRetracting) {
		Cable->SetVisibility(true);
		ShootGoalLocation = AimLocation;
		ShootCurrentLocation = AimLocation;
		CableDirection = (AimLocation - GetActorLocation()).GetSafeNormal();
		//if (GEngine) {
		//	FString Message = FString::Printf(TEXT("%s: ShootDirection is %s"), *(this->GetName()), *CableDirection.ToString());
		//	//0 counts as a unique key, -1 means don't use any key
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, Message);
		//}
		//APhysicsActor* OutPhysicsActor;
		CurrentGrappleShootOutDistance = 0.f;
		CurrentGrappleState = GrappleState::ShootingOut;
	}
}

void APlayerCharacter::StartRetraction()
{
	//TODO remove following
	//EndRetraction();
	if (CurrentGrappleState != GrappleState::Retracted && CurrentGrappleState != GrappleState::FullRetracting) {
		CurrentGrappleState = GrappleState::FullRetracting;
	}
}

void APlayerCharacter::EndRetraction()
{
	Cable->SetVisibility(false);
	Cable->EndLocation = FVector::ZeroVector;
	CurrentGrappleShootOutDistance = 0.f;
	CurrentGrappleState = GrappleState::Retracted;
}

void APlayerCharacter::UpdateCableEndPoint()
{

	FVector CorrectEndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), ShootCurrentLocation);
	Cable->EndLocation = CorrectEndLocation;
	//if (GEngine) {
	//	FString Message = FString::Printf(TEXT("%s: Stuff World %s  Local Cable %s"), *(this->GetName()), *ShootCurrentLocation.ToString(), *CorrectEndLocation.ToString());
	//	//0 counts as a unique key, -1 means don't use any key
	//	GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Green, Message);
	//}
}

void APlayerCharacter::TryToInteract()
{
	if (ObjectToInteractWith && !ObjectToInteractWith->GetIsConsumed()) {
		ObjectToInteractWith->Interact(this);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentFuel = MaxFuel;
	CurrentOxygen = MaxOxygen;
	Cable->EndLocation = FVector::ZeroVector;
	Cable->SetVisibility(false);
	
}

bool APlayerCharacter::GetDidCableConnect()
{
	float SphereSize = 25.f;
	TArray<AActor*> OutActors;
	TArray<AActor*> IgnoreActors;
	TArray<UPrimitiveComponent*> OutComponents;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));


	//DrawDebugSphere(GetWorld(), ShootCurrentLocation, SphereSize, 12, FColor::Yellow, false, 0.1);

	//When using actors
	//bool bHit = UKismetSystemLibrary::SphereOverlapActors(this, ShootCurrentLocation, SphereSize, TraceObjectTypes, AActor::StaticClass(), IgnoreActors, OutActors);
	/*if (bHit) {		
		if (OutActors.Num() > 0) {
			for (AActor* Actor : OutActors) {
				ACustomPhysicsActor* PhysicsActor = Cast<ACustomPhysicsActor>(Actor);
				if (PhysicsActor && !PhysicsActor->GetPreventGrapple()) {
					AttachedPhysicsActor = PhysicsActor;
					return true;
				}
			}
			StartRetraction();
			return false;
		}
		
	}*/

	bool bHit = UKismetSystemLibrary::SphereOverlapComponents(this, ShootCurrentLocation, SphereSize, TraceObjectTypes, UStaticMeshComponent::StaticClass(), IgnoreActors, OutComponents);
	if (bHit) {
		if (OutComponents.Num() > 0) {
			for (UPrimitiveComponent* Component : OutComponents) {
				AActor* Actor = Component->GetOwner();
				ACustomPhysicsActor* PhysicsActor = Cast<ACustomPhysicsActor>(Actor);
				if (PhysicsActor && !PhysicsActor->GetPreventGrapple()) {
					AttachedPhysicsActor = PhysicsActor;
					return true;
				}
			}
			StartRetraction();
			return false;
		}

	}
		//Grapple Functions

	return false;

}

void APlayerCharacter::GrappleTowardsEachOther()
{

	if (AttachedPhysicsActor) {
		float ForceScalar;

		//GetActorLocation is for player because this is player. 
		FVector ActorToPlayerDisplacement = (GetActorLocation() - AttachedPhysicsActor->GetSimulatedBodyLocation());
		//float DistanceSquared = ActorToPlayerDisplacement.SquaredLength();
		float Distance = ActorToPlayerDisplacement.Length();
		FVector ActorToPlayerDirection = (ActorToPlayerDisplacement).GetSafeNormal();
		FVector PlayerToActorDirection = ActorToPlayerDirection * -1.f;

		//Just using gravity formula as an approximation of grapple hook physics, but removing distance from equation may feel better.
		if (bDivideByDistance) {
			//ForceScalar = (GrapplePullConstant * AttachedPhysicsActor->GetMass() * GetMass()) / DistanceSquared;
			ForceScalar = (GrapplePullConstant * AttachedPhysicsActor->GetMass() * GetMass()) / Distance;
		}
		else {
			ForceScalar = (GrapplePullConstantWithNoDivision * AttachedPhysicsActor->GetMass() * GetMass());
		}

		FVector ForceUponPlayer = PlayerToActorDirection * ForceScalar;
		FVector ForceUponActor = ActorToPlayerDirection * ForceScalar;


		AttachedPhysicsActor->AddConstantForce(ForceUponActor);
		AddConstantForce(ForceUponPlayer);
		CableDirection = PlayerToActorDirection;
		CurrentGrappleShootOutDistance = Distance;
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
		//ShootCurrentLocation = AttachedPhysicsActor->GetSimulatedBodyLocation() + AttachedPhysicsActor->OffsetWhenAttached;
		ShootCurrentLocation = AttachedPhysicsActor->GetSimulatedBodyLocation();
		UpdateCableEndPoint();
	}
}

void APlayerCharacter::ExpendFuel(float Amount)
{
	CurrentFuel = FMath::Max(0.f, CurrentFuel- Amount);
}

void APlayerCharacter::ExpendOxygen(float Amount)
{
	CurrentOxygen = FMath::Max(0.f, CurrentOxygen - Amount);
}

void APlayerCharacter::LogMessage(FText& Text)
{
	LogEntries.Add(Text);
}

void APlayerCharacter::SetFuelToMax()
{
	CurrentFuel = MaxFuel;
}

void APlayerCharacter::SetOxygenToMax()
{
	CurrentOxygen = MaxOxygen;
}

void APlayerCharacter::Die_Implementation()
{
	bDead = true;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (GEngine) {
	//	FString Message = FString::Printf(TEXT("%s: Oxygen %f"), *(this->GetName()), CurrentOxygen);
	//	//0 counts as a unique key, -1 means don't use any key
	//	GEngine->AddOnScreenDebugMessage(9, 0.f, FColor::Green, Message);
	//}
	if (!bDead) {
		if (CurrentOxygen <= 0.001f) {
			CurrentOxygen = 0;
			Die();
		}
		else {
			ExpendOxygen(OxygenBaseExpenseRate * DeltaTime);
		}
	}


	switch (CurrentGrappleState){
		case GrappleState::Retracted:

			break;
		case GrappleState::AttachedRetracting:
			GrappleTowardsEachOther();
			SetCableEndpointToAttachment();
			break;

		case GrappleState::ShootingOut:

			if (GEngine) {
				FString Message = FString::Printf(TEXT("%s: Difference %f"), *(this->GetName()), CurrentGrappleShootOutDistance - GrappleShootOutMaxDistance);
				//0 counts as a unique key, -1 means don't use any key
				GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, Message);
			}

			if (FMath::IsNearlyEqual(CurrentGrappleShootOutDistance, GrappleShootOutMaxDistance, 1.0f)) {
				StartRetraction();
				break;
			}

			CurrentGrappleShootOutDistance = FMath::FInterpTo(CurrentGrappleShootOutDistance, GrappleShootOutMaxDistance, DeltaTime, GrappleShootOutSpeed);
			ShootCurrentLocation = RopeStartPivot->GetComponentLocation() + CableDirection * CurrentGrappleShootOutDistance;
			UpdateCableEndPoint();
			if (GetDidCableConnect()) {
				AttachedPhysicsActor->OffsetWhenAttached = ShootCurrentLocation - AttachedPhysicsActor->GetSimulatedBodyLocation();
				AttachedPhysicsActor->OffsetWhenAttached.Y = 0;
				CurrentGrappleState = GrappleState::AttachedRetracting;
			}
			break;
		case GrappleState::FullRetracting:
			if (FMath::IsNearlyZero(CurrentGrappleShootOutDistance, 20.0f)) {
				EndRetraction();
				break;
			}
			CurrentGrappleShootOutDistance = FMath::FInterpTo(CurrentGrappleShootOutDistance, 0, DeltaTime, GrappleFullRetractionSpeed);
			ShootCurrentLocation = RopeStartPivot->GetComponentLocation() + CableDirection * CurrentGrappleShootOutDistance;
			UpdateCableEndPoint();
			break;


		case GrappleState::Attached:

			break;
	}

}

void APlayerCharacter::ApplyThruster(FVector2D Vector)
{
	if (HasFuel() || HasOxygen()) {
		FVector XZForceDirection = FVector(Vector.X, 0, Vector.Y);
		float ForceScalar = 0.f;
		if (bUseAcceleration) {
			ForceScalar = GetMass() * ThrusterAccelertion;
		}
		else {
			ForceScalar = ThrusterForce;
		}

		FVector Force = XZForceDirection * ForceScalar;
		float DeltaSeconds = GetWorld()->GetDeltaSeconds();
		if (!bDebugInfniteFuel) {
			if (HasFuel()) {
				float FuelToSpend = (FuelExpenseRate * FMath::Abs(Vector.X) * DeltaSeconds) + (FuelExpenseRate * FMath::Abs(Vector.Y) * DeltaSeconds);
				ExpendFuel(FuelToSpend);
			}
			else {
				float OxygenToSpend = (OxygenForFuelExpenseRate * FMath::Abs(Vector.X) * DeltaSeconds) + (OxygenForFuelExpenseRate * FMath::Abs(Vector.Y) * DeltaSeconds);
				ExpendOxygen(OxygenToSpend);
			}
		}

		AddConstantForce(Force);
		//if (GEngine) {
		//	FString Message = FString::Printf(TEXT("%s: Applied Force %s"), *(this->GetName()), *Force.ToString());
		//	//0 counts as a unique key, -1 means don't use any key
		//	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, Message);
		//}
	}
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

float APlayerCharacter::GetFuelPercentage()
{
	return CurrentFuel / MaxFuel;
}

float APlayerCharacter::GetOxygenPercentage()
{
	return CurrentOxygen / MaxOxygen;
}

bool APlayerCharacter::HasFuel()
{
	return CurrentFuel > 0;
}

bool APlayerCharacter::HasOxygen()
{
	return CurrentOxygen > 0;
}

bool APlayerCharacter::IsDead()
{
	return bDead;
}

