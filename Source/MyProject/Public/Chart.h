// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "ChartBackgroundComponent.h"
#include "ChartLinesComponent.h"
#include "GameFramework/Actor.h"
#include "Chart.generated.h"

UCLASS()
class MYPROJECT_API AChart : public AActor
{
	GENERATED_BODY()
	
public:	
	AChart();

	//~ Begin API Interface
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void AddLine(int32 Index, const TArray<FVector2D>& InVertices);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void UpdateLine(int32 Index, const TArray<FVector2D>& InVertices);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ClearLine(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ClearAllLines();

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetLineWidth(int32 Index, float Value) { ChartLinesComponent->SetChartLineWidth(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetLineColor(int32 Index, FLinearColor Color) { ChartLinesComponent->SetChartLineColor(Index, Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetLineType(int32 Index, EChartLineType Value) { ChartLinesComponent->SetChartLineType(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetLineDashLength(int32 Index, float Value) { ChartLinesComponent->SetChartLineDashLength(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetLineDashInterval(int32 Index, float Value) { ChartLinesComponent->SetChartLineDashInterval(Index, Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetViewportSize(const FVector2D& ViewportSize);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetRange(const FVector2D& RangeValue);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ScrollLeft(float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ScrollRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ZoomIn(float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ZoomOut(float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void ApplyAutoScale();

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetBackgroundGridType(EChartGridType Type) { ChartBackgroundComponent->SetChartGridType(Type); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetBackgroundColor(const FLinearColor& Color) { ChartBackgroundComponent->SetChartBackgroundColor(Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetBackgroundGridColor(const FLinearColor& Color) { ChartBackgroundComponent->SetChartGridColor(Color); }

	/*UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetTextColor(const FColor& Color) { ChartBackgroundComponent->SetChartTextColor(Color); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetTextSize(float Value) { ChartBackgroundComponent->SetChartTextSize(Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetTitle(const FText& Value) { ChartBackgroundComponent->SetChartTitle(Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetXLabel(const FText& Value) { ChartBackgroundComponent->SetChartXLabel(Value); }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetYLabel(const FText& Value) { ChartBackgroundComponent->SetChartYLabel(Value); }*/
	//~ End API Interface

protected:
	virtual void BeginPlay() override;

private:

private:
	UPROPERTY()
	UChartLinesComponent* ChartLinesComponent;

	UPROPERTY()
	UChartBackgroundComponent* ChartBackgroundComponent;
};
