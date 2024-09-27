// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

/**
 * Tower logic executing every frame.
 */
void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (InFireRange())
    {
        // If in range, rotate turret toward Tank
        RotateTurret(Tank->GetActorLocation());
    }
}

/**
 * 
 */
void ATower::HandleDestruction()
{
    Super::HandleDestruction(); // enable tower visual/sound effects
    
    Destroy(); // destroy tower
}

/**
 * 
 */
void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle, // 
        this, // reference object
        &ATower::CheckFireCondition, // function call
        FireRate, // time interval
        true); // loop condition
}

/**
 * 
 */
void ATower::CheckFireCondition()
{
    if (Tank == nullptr)
    {
        return;
    }
    if (InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

/**
 * Distance check for Tower targeting Tank.
 */
bool ATower::InFireRange()
{
    if (Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        if (Distance <= FireRange)
        {
            return true;
        }        
    }

    return false;
}