// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Called every frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * 
	 */
	void HandleDestruction();

	/**
	 * Public getter for Tank Player Controller.
	 */
	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 800.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 120.f;
	
	void Move(float Value);
	void Turn(float Value);

	APlayerController* TankPlayerController;
};
