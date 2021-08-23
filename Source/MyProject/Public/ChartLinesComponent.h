// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChartLineSection.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ChartLinesComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChartLines, Log, All);

class UProceduralMeshComponent;

/** Component that allows you to draw lines */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UChartLinesComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UChartLinesComponent();

	/**
	 *	Create a chart line section based on procedural mesh.
	 *	@param	LineIndex	Index of the line section to create.
	 *	@param	InVertices	Vertices of all positions to use for this mesh line section.
	 */
	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void CreateChartLine(int32 Index, const TArray<FVector2D>& InVertices);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void ClearChartLine(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void ClearAllChartLines();

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		int32 GetNumChartLines() const;

	/**
	 *	Get pointer to internal data for one section of this lines controller.
	 *	Note that pointer will becomes invalid if sections are added or removed.
	 */
	FLineSection* GetChartLineSection(int32 Index);

	/** Replace a section with new section geometry */
	void SetChartLineSection(int32 Index, const FLineSection& Section);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void SetChartLineVertices(int32 Index, const TArray<FVector2D>& InVertices);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void SetChartLineWidth(int32 Index, float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void SetChartLineColor(int32 Index, FLinearColor Value);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void SetChartLineType(int32 Index, EChartLineType Value);

	/** Set dash length and redraw line */
	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void SetChartLineDashLength(int32 Index, float Value);

	/** Set the length of the spacing between dashes and redraw line */
	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void SetChartLineDashInterval(int32 Index, float Value);

	UFUNCTION(BlueprintCallable, Category = "Chart Line")
		void TrimToChart(const TArray<FVector>& InPlanePositions, const TArray<FVector>& InPlaneNormals);

private:
	void UpdateChartLine(int32 Index, bool bUpdate = false);

private:
	/** Procedural mesh containing line sections */
	UPROPERTY()
		UProceduralMeshComponent* LinesControllerProcMesh;

	UPROPERTY()
		UMaterialInterface* Material;

	UPROPERTY()
		TArray<FLineSection> Lines;

	/** Distance between lines along the z-axis */
	float Step = 0.01f;
};
