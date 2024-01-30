// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SYMBOL_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TQueue<AEnemy*>* GetQueue();

	void SetQueue(TQueue<AEnemy*>* NewQueue);

	FString GetEnemyType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Destroy the actor and remove it from its queue
	UFUNCTION(BlueprintCallable)
	virtual void Die();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	FString EnemyType;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	int Health;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> EnemyMesh;

private:
	TQueue<AEnemy*>* Queue;

};
