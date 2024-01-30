// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "DrawingCanvas.h"
#include "ImageUtility.h"
#include "DefaultGameMode.h"
#include "Projectile.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// On posess?
	if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	if (ADefaultGameMode* GM = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode())) 
	{
		GameMode = GM;
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::Draw(const FInputActionValue& Value)
{
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(this, 0);
	FVector Start = PCM->GetCameraLocation();
	FVector End = PCM->GetActorForwardVector() * 1500.0 + Start;
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(this, Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	ADrawingCanvas* CastTest = Cast<ADrawingCanvas>(HitResult.GetActor());
	if (CastTest)
	{
		Canvas = CastTest;
		FVector2D UV;
		UGameplayStatics::FindCollisionUV(HitResult, 0, UV);
		Canvas->DrawBrush(UV);
		GameMode->DisableAllOtherCanvas(Canvas);
	}

}

void APlayerCharacter::FinishedDrawing(const FInputActionValue& Value)
{
	if (UKismetSystemLibrary::IsValid(Canvas))
	{
		if (!GameMode->IsQueueEmpty(Canvas->GetDirection()))
		{
			FString EnemyType = GameMode->GetEnemyType(Canvas->GetDirection());
			ImageUtility::ExportRenderToImage(this, Canvas->GetRenderTarget());
			int Similarity = ImageUtility::CheckImageSimilarity(EnemyType);
			if (Similarity > 70)
			{
				GameMode->SpawnBullet(Canvas, "Red");
			}
			else if (Similarity > 50) 
			{
				GameMode->SpawnBullet(Canvas, "Yellow");
			}
		}
		GameMode->EnableAllCanvas();
		Canvas->ClearCanvas();
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(DrawAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Draw);
		EnhancedInputComponent->BindAction(DrawAction, ETriggerEvent::Completed, this, &APlayerCharacter::FinishedDrawing);
	}

}
