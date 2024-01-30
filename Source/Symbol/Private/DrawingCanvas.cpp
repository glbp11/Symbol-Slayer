// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingCanvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"

// Sets default values
ADrawingCanvas::ADrawingCanvas()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Plane(TEXT("/Script/Engine.StaticMesh'/Game/DrawingCanvas/Plane.Plane'"));
	if (Plane.Succeeded()) 
	{
		Canvas = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Canvas"));
		SetRootComponent(Canvas);
		Canvas->SetStaticMesh(Plane.Object);
		FRotator rotation = FRotator(0.0f, 0.0f, 90.0f);
		Canvas->SetRelativeRotation(rotation);
		Canvas->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> mat(TEXT("/Script/Engine.Material'/Game/DrawingCanvas/M_Canvas.M_Canvas'"));
	if (mat.Succeeded())
	{
		CanvasMaterial = mat.Object;
	}
	static ConstructorHelpers::FObjectFinder<UTexture> texture(TEXT("/Script/Engine.Texture2D'/Game/DrawingCanvas/BrushColour.BrushColour'"));
	if (texture.Succeeded())
	{
		DrawTexture = texture.Object;
	}
}

// Draws onto Canvas with given texture and size at given location
void ADrawingCanvas::DrawBrush(FVector2D DrawLocation)
{
	UCanvas* tempCanvas;
	FVector2D canvasSize;
	FDrawToRenderTargetContext context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, tempCanvas, canvasSize, context);
	FVector2D screenPosition = (DrawLocation * canvasSize) - (BrushSize / 2.0);
	tempCanvas->K2_DrawTexture(DrawTexture, screenPosition, FVector2D(BrushSize, BrushSize), FVector2D(0, 0), FVector2D(1, 1), FLinearColor::White, BLEND_Opaque);
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, context);
}

// Clears the Canvas
void ADrawingCanvas::ClearCanvas()
{
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, ClearColour);
}

TObjectPtr<UTextureRenderTarget2D> ADrawingCanvas::GetRenderTarget()
{
	return RenderTarget;
}

FName ADrawingCanvas::GetDirection()
{
	return Direction;
}

// Called when the game starts or when spawned
void ADrawingCanvas::BeginPlay()
{
	ClearColour = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	BrushSize = 50.0;
	RenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1024, 1024, RTF_RGBA8, FLinearColor::Black, false, false);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget, ClearColour);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CanvasMaterial, this);
	DynamicMaterial->SetTextureParameterValue("RenderTarget", RenderTarget);
	Canvas->SetMaterial(0, DynamicMaterial);
}