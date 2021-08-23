// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LineHashGrid.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChartStorageComponent.generated.h"

USTRUCT()
struct FLineDataLoader
{
	GENERATED_BODY()

	int32 MaxElementsInCell = 1000;
	int32 MaxCellsInFile = 1000;
	int32 MaxPointsInMem = 1000000;

	double MinCellSizePercent = 0.0001;

	int32 LODsNum;
	int32 PointsNum;

	FBox2D BoundedBox;

	TLineHashGrid<TPair<FVector2D, FVector2D>> LoadedLOD;

	FLineDataLoader(const TArray<FVector2D>& InVertices);

	FLineDataLoader() {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UChartStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChartStorageComponent();

	void AddLine(int32 Index, const TArray<FVector2D>& InVertices);

	void UpdateLine(int32 Index, const TArray<FVector2D>& InVertices);

	void ClearLine(int32 Index, const TArray<FVector2D>& InVertices);

	void ClearAllLines();

private:

};
