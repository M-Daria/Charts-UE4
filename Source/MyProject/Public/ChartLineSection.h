// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ChartLineSection.generated.h"


UENUM()
enum class EChartLineType : uint8
{
	Solid		UMETA(DisplayName = "Solid"),
	Dashed		UMETA(DisplayName = "Dashed"),
	Dotted		UMETA(DisplayName = "Dotted"),
};

/** Struct used to hold the lines and its corresponding properties */
USTRUCT(BlueprintType)
struct FLineSection
{
	GENERATED_BODY()

public:
	UPROPERTY()
		UMaterialInstanceDynamic* DynMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line)
		float ZDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line)
		TArray<FVector2D> Vertices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line)
		float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line)
		FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line)
		EChartLineType Type;

	/** Only for dashed linetype */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line, meta = (EditCondition = "LineType == EChartLineType::Dashed"))
		float DashLength;

	/** Only for dashed linetype. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Line, meta = (EditCondition = "LineType == EChartLineType::Dashed"))
		float DashInterval;

	FLineSection()
		: DynMaterial(nullptr)
		, ZDepth(0.0f)
		, Width(2.f)
		, Type(EChartLineType::Solid)
		, DashLength(10.f)
		, DashInterval(5.f)
	{}

	void Reset()
	{
		DynMaterial = nullptr;
		Vertices.Reset();
		Width = 2.f;
		Type = EChartLineType::Solid;
		DashLength = 10.f;
		DashInterval = 5.f;
	}

	/** Build the vertices of the line taking into account its parameters */
	void BuildLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs);

private:

	void BuildSolidLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs) const;

	void BuildDashedLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs) const;

	void BuildDottedLine(TArray<FVector>& OutVertices, TArray<FVector2D>& OutUVs) const;

	/** Get inner vertices based on line width and line type. If the angle is 180 degrees, it returns false */
	bool BuildInnerVertices(const FVector& Start, const FVector& Middle, const FVector& End, TArray<FVector>& OutVertices) const;
	void BuildOuterVertices(const FVector& Start, const FVector& End, TArray<FVector>& OutVertices, bool bStart) const;

	float AttachToDecimalRange(float Value, const FVector2D& Range) const;
	float AttachToDecimal(float Value, float Decimal) const;

	/** Invert the axes for "vertical" lines position */
	FORCEINLINE FVector InvertAxisYZ(const FVector& Value) const;
};


FORCEINLINE FVector FLineSection::InvertAxisYZ(const FVector& Value) const
{
	return FVector(Value.X, Value.Z, Value.Y);
}