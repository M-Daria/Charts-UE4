// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChartBackgroundComponent.h"
#include "ChartLinesComponent.h"
#include "Components/SceneComponent.h"
#include "ChartViewComponent.generated.h"

USTRUCT()
struct FLine
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector2D> Vertices;

	FVector2D Max;
	FVector2D Min;
	FVector2D MaxInRange;
	FVector2D MinInRange;

	FLine()
	{
		Max = FVector2D(TNumericLimits<float>::Min());
		Min = FVector2D(TNumericLimits<float>::Max());
		MaxInRange = FVector2D(TNumericLimits<float>::Min());
		MinInRange = FVector2D(TNumericLimits<float>::Max());
	}

	void Reset()
	{
		Vertices.Reset();
		Max = FVector2D(TNumericLimits<float>::Min());
		Min = FVector2D(TNumericLimits<float>::Max());
		MaxInRange = FVector2D(TNumericLimits<float>::Min());
		MinInRange = FVector2D(TNumericLimits<float>::Max());
	}

	void UpdateMinMaxInRange(const FVector2D& InRangeX, const FVector2D& InRangeY);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UChartViewComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UChartViewComponent();

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void CreateChartViewport();

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void SetViewportSize(const FVector2D& ViewportSizeValue);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void AddLine(int32 Index, const TArray<FVector2D>& InVertices);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void UpdateLine(int32 Index, const TArray<FVector2D>& InVertices);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void ClearLine(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void ClearAllLines();

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void SetXRange(const FVector2D& XRangeValue);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void SetYRange(const FVector2D& YRangeValue);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void SetXAutoscale(bool bAuto);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport")
	void SetYAutoscale(bool bAuto);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Line Style")
	void SetLineWidth(int32 Index, float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Line Style")
	void SetLineColor(int32 Index, const FLinearColor& Color) { ChartLinesComponent->SetChartLineColor(Index, Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Line Style")
	void SetLineType(int32 Index, EChartLineType Value) { ChartLinesComponent->SetChartLineType(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Line Style")
	void SetLineDashLength(int32 Index, float Value) { ChartLinesComponent->SetChartLineDashLength(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Line Style")
	void SetLineDashInterval(int32 Index, float Value) { ChartLinesComponent->SetChartLineDashInterval(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetChartGridType(EChartGridType Type) { ChartBackgroundComponent->SetChartGridType(Type); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetChartBackgroundColor(const FLinearColor& Color) { ChartBackgroundComponent->SetChartBackgroundColor(Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetChartGridColor(const FLinearColor& Color) { ChartBackgroundComponent->SetChartGridColor(Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetTextColor(EChartTextNames Type, const FColor& Color) { ChartBackgroundComponent->SetTextColor(Type, Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetTextSize(EChartTextNames Type, float Value) { ChartBackgroundComponent->SetTextSize(Type, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetTextTitle(const FText& Value) { ChartBackgroundComponent->SetTextTitle(Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetTextXLabel(const FText& Value) { ChartBackgroundComponent->SetTextXLabel(Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart Viewport|Background Style")
	void SetTextYLabel(const FText& Value) { ChartBackgroundComponent->SetTextYLabel(Value); }

private:
	void Redraw();
	void Trim();

private:
	bool bAutoscaleX = true;
	bool bAutoscaleY = true;

	FVector2D ViewportSize;

	FVector2D XRange;
	FVector2D YRange;

	UPROPERTY()
		TArray<FLine> Lines;

	UPROPERTY()
		UChartBackgroundComponent* ChartBackgroundComponent;

	UPROPERTY()
		UChartLinesComponent* ChartLinesComponent;
};
