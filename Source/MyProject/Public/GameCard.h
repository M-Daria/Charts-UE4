// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameCard.generated.h"

class UChartViewComponent;
class UProceduralMeshComponent;
class UTextRenderComponent;

USTRUCT()
struct FChartParams
{
	GENERATED_BODY()

	UPROPERTY()
	UChartViewComponent* Chart;

	UPROPERTY()
	UMaterialInterface* IconMaterial;

	UPROPERTY()
	UTextRenderComponent* Price;

	FChartParams()
	{
		Chart = nullptr;
		IconMaterial = nullptr;
		Price = nullptr;
	}

	void Reset()
	{
		Chart = nullptr;
		IconMaterial = nullptr;
		Price = nullptr;
	}
};

UCLASS()
class MYPROJECT_API AGameCard : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameCard();

	void AddChart(int32 Index, UMaterialInterface* Material, const FText& BrandName, float Price, const TArray<FVector2D>& Points);

	void GetChart(int32 Index, FChartParams& OutChart);

	void DeleteChart(int32 Index);

	void DeleteAll();

public:
	UPROPERTY()
	TArray<UMaterialInterface*> IconMaterials;

	UPROPERTY()
	TArray<FText> BrandNames;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateBackground();

	UPROPERTY()
	UProceduralMeshComponent* Background;
	UPROPERTY()
	UProceduralMeshComponent* BackgroundCharts;
	UPROPERTY()
	UProceduralMeshComponent* Icons;

	UPROPERTY()
	TArray<FChartParams> Charts;

	UPROPERTY()
	UMaterialInterface* BackgroundMaterial;

private:
	// CHARTS PARAMETERS (OFFSETS AND SIZES)
	bool bIsHorisontal = false;
	int32 ChartsInLine = 3;

	FVector2D BackgroundChartSize = { 675.f, 480.f };
	FVector2D ChartSize = { 475.f, 350.f };
	FVector2D IconSize = { 100.f, 100.f };

	FVector2D OffsetsBetweenCharts = { 20.f, 20.f };
	FVector2D IconOffsetFromTopLeft = { 20.f, 20.f };
	FVector2D ChartOffsetFromBottomRight = { 20.f, 50.f };
	// CHARTS PARAMETERS (OFFSETS AND SIZES)

	// COLOR PALETTE
	UPROPERTY()
	FLinearColor BackgroundTopColor = FLinearColor(0.003922f, 0.f, 0.301961f);
	UPROPERTY()
	FLinearColor BackgroundBottomColor = FLinearColor::Black;
	UPROPERTY()
	FLinearColor ChartsBackgroundTopColor;
	UPROPERTY()
	FLinearColor ChartsBackgroundBottomColor;
	UPROPERTY()
	FLinearColor ChartBackgroundColor = FLinearColor(0.0f, 0.0f, 0.023529f);
	UPROPERTY()
	FLinearColor ChartGridColor = FLinearColor(0.0f, 0.0f, 0.008333f);
	UPROPERTY()
	FLinearColor TextPriceColor = FLinearColor::White;
	UPROPERTY()
	FLinearColor TextBrandNameColor = FLinearColor(0.583333f, 0.583333f, 0.583333f);
	UPROPERTY()
	FLinearColor TextAxisValuesColor = FLinearColor(0.1075f, 0.109063f, 0.2f);
	// COLOR PALETTE
};
