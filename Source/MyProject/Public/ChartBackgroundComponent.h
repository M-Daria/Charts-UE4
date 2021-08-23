// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ChartBackgroundComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChartBackground, Log, All);

class UProceduralMeshComponent;
class UTextRenderComponent;

/** Enum used to hold background styles */
UENUM()
enum class EChartGridType : uint8
{
	NoGrid				UMETA(DisplayName = "No Grid"),
	Solid				UMETA(DisplayName = "Solid"),
	SolidHorizontal		UMETA(DisplayName = "Solid Horizontal"),
	SolidVertical		UMETA(DisplayName = "Solid Vertical"),
	Dashed				UMETA(DisplayName = "Dashed"),
	DashedHorizontal	UMETA(DisplayName = "Dashed Horizontal"),
	DashedVertical		UMETA(DisplayName = "Dashed Vertical"),
	StripedHorizontal	UMETA(DisplayName = "Striped Horizontal"),
	StripedVertical		UMETA(DisplayName = "Striped Vertical"),
};

/** Enum is used to store the names of parts of the background text */
UENUM()
enum class EChartTextNames : uint8
{
	Title = 0		UMETA(DisplayName = "Title"),
	XLabel = 1		UMETA(DisplayName = "X Label"),
	YLabel = 2		UMETA(DisplayName = "Y Label"),
	AxisValues = 3	UMETA(DisplayName = "Axis Values"),
};


/** Component that allows you to draw background of the chart */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UChartBackgroundComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UChartBackgroundComponent();

	/**
	 *	Create a chart background based on procedural mesh.
	 *	@param	XRange		Range of displayed values of the x-axis.
	 *	@param	YRange		Range of displayed values of the y-axis.
	 */
	UFUNCTION(BlueprintCallable, Category = "Background", meta = (AutoCreateRefTerm = "XRangeValue,YRangeValue"))
	void CreateChartBackground(const FVector2D& XRangeValue, const FVector2D& YRangeValue);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetChartBackgroundXRange(const FVector2D& XRangeValue);
	UFUNCTION(BlueprintCallable, Category = "Background")
		FVector2D GetChartBackgroundXRange() const { return XRange; }

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetChartBackgroundYRange(const FVector2D& YRangeValue);
	UFUNCTION(BlueprintCallable, Category = "Background")
		FVector2D GetChartBackgroundYRange() const { return YRange; }

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetChartBackgroundViewportSize(const FVector2D& ViewportSize);
	UFUNCTION(BlueprintCallable, Category = "Background")
		FVector2D GetChartBackgroundViewportSize() const { return BackgroundViewportSize; }

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetChartGridType(EChartGridType Type);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetChartBackgroundColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetChartGridColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetTextColor(EChartTextNames Type, const FColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetTextSize(EChartTextNames Type, float Value);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetTextTitle(const FText& Value);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetTextXLabel(const FText& Value);

	UFUNCTION(BlueprintCallable, Category = "Background")
		void SetTextYLabel(const FText& Value);

private:
	/** Enum used to store names of the background parts' meshes */
	enum EBackgroundPartNames : uint8
	{
		Background = 0,
		Grid = 1,
		Frame = 2,
		// Ticks = 3
	};

	void DrawChartBackground();
	/** Build vertices of the background part and its rendering */
	void DrawBackgroundPart(EBackgroundPartNames Part, const TArray<FVector2D>& UVs);

	/** Draw axis labels and chart title */
	void RenderAxisValuesText(const FVector2D& ClampedStart, const FVector2D& Start, const FVector2D& Step);

	void SetMaterialGridParams(EBackgroundPartNames Part, const FVector2D& VertWidthOffset, const FVector2D& HorWidthOffset, float bSecondGridLayer = 0.0);

	void BuildSolidUVs(TArray<FVector2D>& UV0s, TArray<FVector2D>& UV1s) const;
	void BuildSolidUVs(TArray<FVector2D>& UVs, bool bHorisontal = false) const;
	void BuildDashedUVs(TArray<FVector2D>& UV0s, TArray<FVector2D>& UV1s) const;
	void BuildDashedUVs(TArray<FVector2D>& UVs, bool bHorisontal = false) const;
	void BuildStripedUVs(TArray<FVector2D>& UVs, bool bHorisontal = false) const;

	/** Get the distance between the grid lines */
	float GetGridStep(const FVector2D& Range) const;
	/** Get the offset of the grid lines from the edge of the background */
	float GetGridOffset(float Start, float Step) const;
	/** Get the closest number with a given decimal part */
	float AttachToDecimal(float Value, float Decimal) const;

	/** Procedural mesh containing background parts */
	UPROPERTY()
		UProceduralMeshComponent* BackgroundControllerProcMesh;

	UPROPERTY()
		UMaterialInterface* Material;
	UPROPERTY()
		TArray<UMaterialInstanceDynamic*> DynMaterials;

	UPROPERTY()
		TArray<UTextRenderComponent*> AxisValues;
	UPROPERTY()
		TArray<UTextRenderComponent*> TextParts;

	FVector2D StepClamped;

	int32 MaxGridLinesCount = 22;
	float GridLineWidth = 0.002;
	float GridBandLength = 0.5;
	float GridDashLength = 0.03;
	float GridDashInterval = 0.1;

	int32 MaximumFractionalDigits = 4;

	float TextOffset = 5.0;

	float DistBackgroundParts = 0.05;

	float FrameWidth = 1.0;
	FLinearColor FrameColor = FLinearColor::Black;

private:
	UPROPERTY()
		FVector2D BackgroundViewportSize;

	UPROPERTY()
		FVector2D XRange;
	UPROPERTY()
		FVector2D YRange;

	UPROPERTY()
		EChartGridType GridType;

	UPROPERTY()
		FLinearColor BackgroundColor;
	UPROPERTY()
		FLinearColor GridColor;
};
