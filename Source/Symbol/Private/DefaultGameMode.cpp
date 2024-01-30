// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "DrawingCanvas.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "Projectile.h"

ADefaultGameMode::ADefaultGameMode()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADrawingCanvas::StaticClass(), CanvasArray);
	for (AActor* Canvas : CanvasArray)
	{
		ADrawingCanvas* CurrentCanvas = Cast<ADrawingCanvas>(Canvas);
		FName CanvasDirection = CurrentCanvas->GetDirection();
		if (CanvasDirection == "North")
		{
			EnemyMap.Add(CanvasDirection, &EnemiesNorth);
		}
		else if (CanvasDirection == "East")
		{
			EnemyMap.Add(CanvasDirection, &EnemiesEast);
		}
		else if (CanvasDirection == "South")
		{
			EnemyMap.Add(CanvasDirection, &EnemiesSouth);
		}
		else
		{
			EnemyMap.Add(CanvasDirection, &EnemiesWest);
		}
	}

}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultGameMode::DisableAllOtherCanvas(ADrawingCanvas* Canvas)
{
	FName ActiveCanvas = Canvas->GetDirection();
	for (AActor* TempCanvas : CanvasArray)
	{
		ADrawingCanvas* CurrentCanvas = Cast<ADrawingCanvas>(TempCanvas);
		if (CurrentCanvas)
		{
			FName OtherCanvas = CurrentCanvas->GetDirection();
			CurrentCanvas->SetActorEnableCollision(ActiveCanvas == OtherCanvas);
		}
	}

}

void ADefaultGameMode::EnableAllCanvas()
{
	for (AActor* TempCanvas : CanvasArray)
	{
		ADrawingCanvas* CurrentCanvas = Cast<ADrawingCanvas>(TempCanvas);
		CurrentCanvas->SetActorEnableCollision(true);
	}

}

AEnemy* ADefaultGameMode::SpawnEnemies(TSubclassOf<AEnemy> SpawnClass, FVector SpawnLocation, FName Direction)
{
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);
	AEnemy* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AEnemy>(SpawnClass, SpawnTransform);

	TQueue<AEnemy*>* EnemyQueue = *(EnemyMap.Find(Direction));

	if (UKismetSystemLibrary::IsValid(SpawnedEnemy))
	{
		EnemyQueue->Enqueue(SpawnedEnemy);
		SpawnedEnemy->SetQueue(EnemyQueue);
		SpawnedEnemy->FinishSpawning(SpawnTransform);
		return SpawnedEnemy;
	}
	return nullptr;

}

void ADefaultGameMode::SpawnBullet(ADrawingCanvas* Canvas, FName ProjectileType)
{
	FVector SpawnLocation = FVector(1600.00f, 850.0f, 120.0f);
	if (Canvas) {
		FRotator FireRotation = Canvas->GetActorRotation().Add(0, -90, 0);
		SpawnLocation = Canvas->GetActorTransform().GetLocation() + UKismetMathLibrary::GetForwardVector(FireRotation);
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		if (UKismetSystemLibrary::IsValid(RedProjectileClass) && UKismetSystemLibrary::IsValid(YellowProjectileClass))
		{
			AProjectile* SpawnedProjectile = nullptr;
			if (ProjectileType == "Red")
			{
				UE_LOG(LogTemp, Warning, TEXT("Red"));
				SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(RedProjectileClass, SpawnLocation, SpawnRotation);
			}
			else if (ProjectileType == "Yellow")
			{
				UE_LOG(LogTemp, Warning, TEXT("Yellow"));
				SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(YellowProjectileClass, SpawnLocation, SpawnRotation);
			}
			
			if (UKismetSystemLibrary::IsValid(SpawnedProjectile))
			{
	
				FVector FireDirection = FireRotation.Vector();
				SpawnedProjectile->FireInDirection(FireDirection);
				TQueue<AEnemy*>* EnemyQueue = *(EnemyMap.Find(Canvas->GetDirection()));
				AEnemy* CloseEnemy = (*(*EnemyQueue).Peek());
				if (UKismetSystemLibrary::IsValid(CloseEnemy))
				{
					SpawnedProjectile->ProjectileMovementComponent->HomingTargetComponent = CloseEnemy->GetRootComponent();
				}
				
			}
		}
	}
	
}

bool ADefaultGameMode::IsQueueEmpty(FName Direction)
{
	TQueue<AEnemy*>* EnemyQueue = *(EnemyMap.Find(Direction));
	return EnemyQueue->IsEmpty();

}

FString ADefaultGameMode::GetEnemyType(FName Direction)
{
	TQueue<AEnemy*>* EnemyQueue = *(EnemyMap.Find(Direction));
	AEnemy* CloseEnemy = (*(*EnemyQueue).Peek());
	return CloseEnemy->GetEnemyType();

}
