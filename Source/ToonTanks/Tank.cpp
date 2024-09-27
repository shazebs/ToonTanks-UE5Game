// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

/**
 * 
 */
ATank::ATank()
{
    // attach Spring Arm to Tank Root
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    // attach Camera to Spring Arm
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

/**
 * Set user controls for playable characters.
 */
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind player key bindings
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

/**
 * 
 */
void ATank::BeginPlay()
{
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
}

/**
 * Called every frame.
 */
void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult HitResult;

        // mouse position object
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false, 
            HitResult);

        RotateTurret(HitResult.ImpactPoint);

        /* Draw a Debug Sphere where Player Tank's mouse is pointing
        DrawDebugSphere(
            GetWorld(), 
            HitResult.ImpactPoint, // sphere position
            25.f, // radius of sphere
            12, // number of segments
            FColor::Red, // sphere color
            false, // smooth lines
            -1.f); // seconds visible */
    }
}

/**
 * Handle destruction of Tank pawn.
 */
void ATank::HandleDestruction()
{
    Super::HandleDestruction();    
    SetActorHiddenInGame(true); // hide Tank as to not destroy Player Start
    SetActorTickEnabled(false); // turn off Tank tick function
    bAlive = false;
}

/**
 * W and S key bindings for traversing forward and backward.
 */
void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
}

/**
 * A and D key bindings for rotating left and right.
 */
void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}