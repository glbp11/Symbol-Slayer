// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Containers/Queue.h"
#include "DefaultGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SYMBOL_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	ADefaultGameMode();

	/** Disables collision so other canvases cannot be drawn on */
	UFUNCTION(BlueprintCallable)
	void DisableAllOtherCanvas(class ADrawingCanvas* Canvas);

	/** Enables collision back on all the canvases */
	UFUNCTION(BlueprintCallable)
	void EnableAllCanvas();

	/** Spawns given enemy at given location and direction */
	UFUNCTION(BlueprintCallable)
	AEnemy* SpawnEnemies(TSubclassOf<AEnemy> SpawnClass, FVector SpawnLocation, FName Direction);

	/** Spawns bullet in direction of canvas of given type */
	UFUNCTION(BlueprintCallable)
	void SpawnBullet(ADrawingCanvas* Canvas, FName ProjectileType);

	bool IsQueueEmpty(FName Direction);

	FString GetEnemyType(FName Direction);

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Array of all four Canvas Actors */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<AActor*> CanvasArray;

	/** Queues containing enemies coming from each direction */
	TQueue<class AEnemy*> EnemiesNorth;
	TQueue<AEnemy*> EnemiesEast;
	TQueue<AEnemy*> EnemiesSouth;
	TQueue<AEnemy*> EnemiesWest;

	/** Map of all enemy queues for easier access */
	TMap<FName, TQueue<AEnemy*>*> EnemyMap;

	/** Bullet class to instantly kill enemies */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> RedProjectileClass;

	/** Bullet class to damage enemies */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> YellowProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	bool IsGameOver;

};
