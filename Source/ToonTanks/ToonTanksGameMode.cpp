// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

/**
 * 
 */
void AToonTanksGameMode::ActorDied(AActor *DeadActor)
{
    // Handle Tank destruction
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction(); // destroy Tank
        
        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }

        GameOver(false);
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {        
        DestroyedTower->HandleDestruction(); 
        TargetTowers--;

        if (TargetTowers == 0)
        {
            GameOver(true);
        }
    }
}

/**
 * 
 */
void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

/**
 * 
 */
void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;        
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject( // callback function
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true
        );

        GetWorldTimerManager().SetTimer( // call timer 
            PlayerEnableTimerHandle, 
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

/**
 * Return count of remaining Towers alive in the game.
 */
int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}