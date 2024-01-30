// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SYMBOL_API ImageUtility
{
public:
	ImageUtility();
	~ImageUtility();

/*	Compare Reference image with Player's drawn image and return the similarity between them.
 *	Returns -1 on failure to read either images
 */
	static int CheckImageSimilarity(FString EnemyType);

/*  Export the given RenderTarget as RenderImage.png to the Assets folder */
	static void ExportRenderToImage(UObject* Object, UTextureRenderTarget2D* RenderTarget);
};
