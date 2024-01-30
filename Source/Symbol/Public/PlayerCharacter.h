// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "DrawingCanvas.h"

#include "PlayerCharacter.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SYMBOL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void Look(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Draw(const FInputActionValue& Value);

	UFUNCTION()
	virtual void FinishedDrawing(const FInputActionValue& Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputAction* DrawAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	class ADefaultGameMode* GameMode;

private:
	TObjectPtr<ADrawingCanvas> Canvas;

};