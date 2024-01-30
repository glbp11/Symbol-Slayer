// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"

#include "DrawingCanvas.generated.h"

UCLASS(Blueprintable, BlueprintType, placeable)
class SYMBOL_API ADrawingCanvas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawingCanvas();

	/** Draws onto Canvas with given texture and size at given location */
	void DrawBrush(FVector2D DrawLocation);

	/** Clears the Canvas */
	void ClearCanvas();

	TObjectPtr<UTextureRenderTarget2D> GetRenderTarget();

	FName GetDirection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** RenderTarget used for drawing */
	TObjectPtr<UTextureRenderTarget2D> RenderTarget;

	/** Direction of the Canvas */
	UPROPERTY(EditInstanceOnly, Category = "Direction")
	FName Direction;

	/** Canvas used for drawing */
	TObjectPtr<UStaticMeshComponent> Canvas;

	/** Material of the Canvas */
	UMaterial* CanvasMaterial;

	/** Colour used to clear the Canvas */
	FLinearColor ClearColour;

	/** Texture being drawn to Canvas */
	UTexture* DrawTexture;

	/** Size of texture being drawn */
	double BrushSize;
};