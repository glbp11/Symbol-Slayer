// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawingCanvas.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AAIController* EnemyController = Cast<AAIController>(GetController());
	if (UKismetSystemLibrary::IsValid(EnemyController)) {
		EnemyController->MoveToActor((UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));
	}

}

// Destroy the actor and remove it from its queue
void AEnemy::Die()
{
	Queue->Pop();
	Destroy();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector StartVector = GetActorLocation();
	FVector EndVector = FVector(StartVector.X, StartVector.Y, 0);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	bool TraceSuccess = UKismetSystemLibrary::LineTraceSingle(this, StartVector, EndVector, UEngineTypes::ConvertToTraceType(ECC_Visibility), 
																false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	if (TraceSuccess) 
	{
		UCapsuleComponent* EnemyCapsule = GetCapsuleComponent();
		FVector Normal = HitResult.ImpactNormal;
		FRotator ComponentRotation = EnemyCapsule->USceneComponent::GetComponentRotation();
		FRotator XRotation = UKismetMathLibrary::MakeRotFromXZ(EnemyCapsule->USceneComponent::GetForwardVector(), Normal);
		FRotator YRotation = UKismetMathLibrary::MakeRotFromYZ(EnemyCapsule->USceneComponent::GetRightVector(), Normal);
		FRotator Target = FRotator(YRotation.Pitch, ComponentRotation.Yaw, XRotation.Roll);
		FRotator Result = UKismetMathLibrary::RInterpTo(ComponentRotation, Target, UGameplayStatics::GetWorldDeltaSeconds(this), 50.0f);
		EnemyCapsule->USceneComponent::SetWorldRotation(Result);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TQueue<AEnemy*>* AEnemy::GetQueue()
{
	return Queue;
}

void AEnemy::SetQueue(TQueue<AEnemy*>* NewQueue)
{
	Queue = NewQueue;
}

FString AEnemy::GetEnemyType()
{
	return EnemyType;
}
