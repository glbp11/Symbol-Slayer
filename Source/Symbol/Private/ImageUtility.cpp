// Fill out your copyright notice in the Description page of Project Settings.


#include "ImageUtility.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Misc/Paths.h"

#include "PreOpenCVHeaders.h"
#include "OpenCVHelper.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "PostOpenCVHeaders.h"

ImageUtility::ImageUtility()
{
}

ImageUtility::~ImageUtility()
{
}

/*	Compare Reference image with Player's drawn image and return the similarity between them.
 *	Returns -1 on failure to read either images
 */
int ImageUtility::CheckImageSimilarity(FString EnemyType)
{
	FString FRenderPath = FPaths::ProjectContentDir() + FString("Assets/RenderImage.png");
	FRenderPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FRenderPath);
	std::string RenderPath(TCHAR_TO_UTF8(*FRenderPath));
	cv::Mat RenderImage = cv::imread(RenderPath, cv::IMREAD_GRAYSCALE);

	FString FReferencePath = FPaths::ProjectContentDir() + FString("Assets/") + EnemyType + FString(".png");
	FReferencePath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FReferencePath);
	std::string ReferencePath(TCHAR_TO_UTF8(*FReferencePath));
	cv::Mat ReferenceImage = cv::imread(ReferencePath, cv::IMREAD_GRAYSCALE);

	if (RenderImage.empty() || ReferenceImage.empty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Image not read properly"));
		return -1;
	}
	cv::Mat Difference;
	cv::subtract(ReferenceImage, RenderImage, Difference);
	int TotalNonZeros = cv::countNonZero(Difference);

	// Reference Image and Rendered Image after subtraction needs to be thresholded to get relevant pixels
	cv::Mat ThresholdAppliedMat;
	cv::threshold(Difference, ThresholdAppliedMat, 100, 255, cv::THRESH_BINARY);

	int PixelsMissed = cv::countNonZero(ThresholdAppliedMat);
	float FractionMissed = static_cast<float>(PixelsMissed)/static_cast<float>(TotalNonZeros);
	float PercentFilled = (1 - FractionMissed) * 100;

	cv::Mat TemplateMatching;
	double minVal;
	double maxVal;

	cv::matchTemplate(ReferenceImage, RenderImage, TemplateMatching, cv::TM_CCORR_NORMED);
	cv::minMaxLoc(TemplateMatching, &minVal, &maxVal);
	float TemplateSimilarity = maxVal * 100;

	// Take a blend of similarity from template matching and how much of the user's drawn image was drawn over the reference image
	float Result = 0.3*PercentFilled + 0.7*TemplateSimilarity;
	UE_LOG(LogTemp, Log, TEXT("Result Similar: %d"), (int)Result);
	return (int)Result;
}

/*  Export the given RenderTarget as RenderImage.png to the Assets folder */
void ImageUtility::ExportRenderToImage(UObject* Object, UTextureRenderTarget2D* RenderTarget)
{
	UKismetRenderingLibrary::ExportRenderTarget(Object, RenderTarget, FPaths::ProjectContentDir() + FString("Assets"), FString("RenderImage.png"));
}
