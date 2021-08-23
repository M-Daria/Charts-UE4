// Fill out your copyright notice in the Description page of Project Settings.


#include "ChartViewComponent.h"

UChartViewComponent::UChartViewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ChartBackgroundComponent = CreateDefaultSubobject<UChartBackgroundComponent>(TEXT("View Background Component"));
	ChartLinesComponent = CreateDefaultSubobject<UChartLinesComponent>(TEXT("View Lines Component"));

	ChartBackgroundComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ChartLinesComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ChartLinesComponent->AddLocalOffset(FVector(0.0, 0.1, 0.0));

	ViewportSize = FVector2D(500, 500);
	XRange = FVector2D(0, 5);
	YRange = FVector2D(0, 5);
}

void UChartViewComponent::CreateChartViewport()
{
	ChartBackgroundComponent->CreateChartBackground(XRange, YRange);
	ChartBackgroundComponent->SetChartBackgroundViewportSize(ViewportSize);
}

void UChartViewComponent::SetViewportSize(const FVector2D& ViewportSizeValue)
{
	if (ViewportSizeValue.X >= 10 && ViewportSizeValue.Y >= 10)
	{
		ViewportSize = ViewportSizeValue;
		ChartBackgroundComponent->SetChartBackgroundViewportSize(ViewportSize);

		Redraw();
	}
}

void UChartViewComponent::AddLine(int32 Index, const TArray<FVector2D>& InVertices)
{
	if (Index >= 0)
	{
		// Ensure lines array is long enough
		if (Index >= Lines.Num())
			Lines.SetNum(Index + 1, false);

		auto& NewLine = Lines[Index];
		NewLine.Reset();
		NewLine.Vertices = InVertices;

		NewLine.UpdateMinMaxInRange(XRange, YRange);

		TArray<FVector2D> EmptyArray;
		ChartLinesComponent->CreateChartLine(Index, EmptyArray);

		Redraw();
	}
}

void UChartViewComponent::UpdateLine(int32 Index, const TArray<FVector2D>& InVertices)
{
	if (Index < Lines.Num() && Index >= 0)
	{
		Lines[Index].Reset();
		Lines[Index].Vertices = InVertices;
		Lines[Index].UpdateMinMaxInRange(XRange, YRange);

		Redraw();
	}
}

void UChartViewComponent::ClearLine(int32 Index)
{
	if (Index < Lines.Num() && Index >= 0)
	{
		Lines[Index].Reset();
		ChartLinesComponent->ClearChartLine(Index);

		Redraw();
	}
}

void UChartViewComponent::ClearAllLines()
{
	Lines.Reset();
	ChartLinesComponent->ClearAllChartLines();
}

void UChartViewComponent::SetXRange(const FVector2D& XRangeValue)
{
	if (XRangeValue.X < XRangeValue.Y)
	{
		XRange = XRangeValue;

		for (auto& Line : Lines)
			Line.UpdateMinMaxInRange(XRange, YRange);

		Redraw();
	}
}

void UChartViewComponent::SetYRange(const FVector2D& YRangeValue)
{
	if (YRangeValue.X < YRangeValue.Y)
	{
		YRange = YRangeValue;

		for (auto& Line : Lines)
			Line.UpdateMinMaxInRange(XRange, YRange);

		Redraw();
	}
}

void UChartViewComponent::SetXAutoscale(bool bAuto)
{
	bAutoscaleX = bAuto;
	Redraw();
}

void UChartViewComponent::SetYAutoscale(bool bAuto)
{
	bAutoscaleY = bAuto;
	Redraw();
}


void UChartViewComponent::SetLineWidth(int32 Index, float Value)
{
	if (Index >= 0 && Value > 0.0)
	{
		ChartLinesComponent->SetChartLineWidth(Index, Value);
		Trim();
	}
}


void UChartViewComponent::Redraw()
{
	TArray<FVector2D> ClampedLine;
	FVector2D Max = FVector2D(TNumericLimits<float>::Min());
	FVector2D Min = FVector2D(TNumericLimits<float>::Max());
	FVector2D MaxInRange = FVector2D(TNumericLimits<float>::Min());
	FVector2D MinInRange = FVector2D(TNumericLimits<float>::Max());

	for (int i = 0; i < Lines.Num(); i++)
	{
		if (Lines[i].Max.X > Max.X) Max.X = Lines[i].Max.X;
		if (Lines[i].Min.X < Min.X) Min.X = Lines[i].Min.X;

		if (Lines[i].Max.Y > Max.Y) Max.Y = Lines[i].Max.Y;
		if (Lines[i].Min.Y < Min.Y) Min.Y = Lines[i].Min.Y;

		if (Lines[i].MaxInRange.X > MaxInRange.X) MaxInRange.X = Lines[i].MaxInRange.X;
		if (Lines[i].MinInRange.X < MinInRange.X) MinInRange.X = Lines[i].MinInRange.X;

		if (Lines[i].MaxInRange.Y > MaxInRange.Y) MaxInRange.Y = Lines[i].MaxInRange.Y;
		if (Lines[i].MinInRange.Y < MinInRange.Y) MinInRange.Y = Lines[i].MinInRange.Y;
	}

	if (FMath::IsNearlyEqual(MaxInRange.X, MinInRange.X))
	{
		MaxInRange.X += 0.5;
		MinInRange.X -= 0.5;
	}
	if (FMath::IsNearlyEqual(MaxInRange.Y, MinInRange.Y))
	{
		MaxInRange.Y += 0.5;
		MinInRange.Y -= 0.5;
	}
	if (FMath::IsNearlyEqual(Max.X, Min.X))
	{
		Max.X += 0.5;
		Min.X -= 0.5;
	}
	if (FMath::IsNearlyEqual(Max.Y, Min.Y))
	{
		Max.Y += 0.5;
		Min.Y -= 0.5;
	}

	if (bAutoscaleX && bAutoscaleY)
	{
		ChartBackgroundComponent->SetChartBackgroundXRange(FVector2D(Min.X, Max.X));
		ChartBackgroundComponent->SetChartBackgroundYRange(FVector2D(Min.Y, Max.Y));
	}
	else if (bAutoscaleX)
	{
		ChartBackgroundComponent->SetChartBackgroundXRange(FVector2D(MinInRange.X, MaxInRange.X));
		ChartBackgroundComponent->SetChartBackgroundYRange(FVector2D(YRange.X, YRange.Y));
	}
	else if (bAutoscaleY)
	{
		ChartBackgroundComponent->SetChartBackgroundXRange(FVector2D(XRange.X, XRange.Y));
		ChartBackgroundComponent->SetChartBackgroundYRange(FVector2D(MinInRange.Y, MaxInRange.Y));
	}
	else
	{
		ChartBackgroundComponent->SetChartBackgroundXRange(FVector2D(XRange.X, XRange.Y));
		ChartBackgroundComponent->SetChartBackgroundYRange(FVector2D(YRange.X, YRange.Y));
	}

	for (int i = 0; i < Lines.Num(); i++)
	{
		ClampedLine.Reset();
		for (auto Vertex : Lines[i].Vertices)
		{
			FVector2D ClampedVertex;
			if (bAutoscaleX && bAutoscaleY)
			{
				ClampedVertex.X = FMath::GetMappedRangeValueUnclamped(FVector2D(Min.X, Max.X), FVector2D(0.0, ViewportSize.X), Vertex.X);
				ClampedVertex.Y = FMath::GetMappedRangeValueUnclamped(FVector2D(Min.Y, Max.Y), FVector2D(0.0, ViewportSize.Y), Vertex.Y);
			}
			else if (bAutoscaleX)
			{
				ClampedVertex.X = FMath::GetMappedRangeValueUnclamped(FVector2D(MinInRange.X, MaxInRange.X), FVector2D(0.0, ViewportSize.X), Vertex.X);
				ClampedVertex.Y = FMath::GetMappedRangeValueUnclamped(FVector2D(YRange.X, YRange.Y), FVector2D(0.0, ViewportSize.Y), Vertex.Y);
			}
			else if (bAutoscaleY)
			{
				ClampedVertex.X = FMath::GetMappedRangeValueUnclamped(FVector2D(XRange.X, XRange.Y), FVector2D(0.0, ViewportSize.X), Vertex.X);
				ClampedVertex.Y = FMath::GetMappedRangeValueUnclamped(FVector2D(MinInRange.Y, MaxInRange.Y), FVector2D(0.0, ViewportSize.Y), Vertex.Y);
			}
			else
			{
				ClampedVertex.X = FMath::GetMappedRangeValueUnclamped(FVector2D(XRange.X, XRange.Y), FVector2D(0.0, ViewportSize.X), Vertex.X);
				ClampedVertex.Y = FMath::GetMappedRangeValueUnclamped(FVector2D(YRange.X, YRange.Y), FVector2D(0.0, ViewportSize.Y), Vertex.Y);
			}
			ClampedLine.Emplace(ClampedVertex);
		}
		ChartLinesComponent->SetChartLineVertices(i, ClampedLine);
	}

	Trim();
}

void UChartViewComponent::Trim()
{
	TArray<FVector> PlanePositons, PlaneNormals;
	FVector WorldLocation = ChartBackgroundComponent->GetComponentLocation();
	WorldLocation.X = WorldLocation.X + 1.0; WorldLocation.Z = WorldLocation.Z + 1.0;
	FVector ViewportSize3D(ViewportSize.X - 2.0, 0.0, ViewportSize.Y - 2.0);

	PlanePositons.Emplace(WorldLocation);
	PlanePositons.Emplace(WorldLocation);
	PlanePositons.Emplace(WorldLocation + TransformVector(GetComponentTransform(), ViewportSize3D));
	PlanePositons.Emplace(WorldLocation + TransformVector(GetComponentTransform(), ViewportSize3D));
	PlaneNormals.Emplace(TransformVector(GetComponentTransform(), FVector(1.0, 0.0, 0.0)));
	PlaneNormals.Emplace(TransformVector(GetComponentTransform(), FVector(0.0, 0.0, 1.0)));
	PlaneNormals.Emplace(TransformVector(GetComponentTransform(), FVector(-1.0, 0.0, 0.0)));
	PlaneNormals.Emplace(TransformVector(GetComponentTransform(), FVector(0.0, 0.0, -1.0)));
	ChartLinesComponent->TrimToChart(PlanePositons, PlaneNormals);
}


void FLine::UpdateMinMaxInRange(const FVector2D& InRangeX, const FVector2D& InRangeY)
{
	Max = FVector2D(TNumericLimits<float>::Min());
	Min = FVector2D(TNumericLimits<float>::Max());
	MaxInRange = FVector2D(TNumericLimits<float>::Min());
	MinInRange = FVector2D(TNumericLimits<float>::Max());

	for (int i = 0; i < Vertices.Num(); i++)
	{
		if (Vertices[i].X > Max.X) Max.X = Vertices[i].X;
		if (Vertices[i].X < Min.X) Min.X = Vertices[i].X;

		if (Vertices[i].Y > Max.Y) Max.Y = Vertices[i].Y;
		if (Vertices[i].Y < Min.Y) Min.Y = Vertices[i].Y;

		if (InRangeX.X <= Vertices[i].X && Vertices[i].X <= InRangeX.Y)
		{
			FVector2D BoundLeft(Vertices[i].Y), BoundRight(Vertices[i].Y);

			if (i != 0)
			{
				if (Vertices[i - 1].X < InRangeX.X)
					BoundLeft = Vertices[i - 1] + (Vertices[i] - Vertices[i - 1]) * (InRangeX.X - Vertices[i - 1].X) / (Vertices[i].X - Vertices[i - 1].X);
				else if (Vertices[i - 1].X > InRangeX.Y)
					BoundRight = Vertices[i] + (Vertices[i - 1] - Vertices[i]) * (InRangeX.Y - Vertices[i].X) / (Vertices[i - 1].X - Vertices[i].X);
			}
			if (i != Vertices.Num() - 1)
			{
				if (Vertices[i + 1].X < InRangeX.X)
					BoundLeft = Vertices[i + 1] + (Vertices[i] - Vertices[i + 1]) * (InRangeX.X - Vertices[i + 1].X) / (Vertices[i].X - Vertices[i + 1].X);
				else if (Vertices[i + 1].X > InRangeX.Y)
					BoundRight = Vertices[i] + (Vertices[i + 1] - Vertices[i]) * (InRangeX.Y - Vertices[i].X) / (Vertices[i + 1].X - Vertices[i].X);
			}
			float max3 = FMath::Max3(BoundLeft.Y, Vertices[i].Y, BoundRight.Y);
			float min3 = FMath::Min3(BoundLeft.Y, Vertices[i].Y, BoundRight.Y);

			if (max3 > MaxInRange.Y) MaxInRange.Y = max3;
			if (min3 < MinInRange.Y) MinInRange.Y = min3;
		}


		if (InRangeY.X <= Vertices[i].Y && Vertices[i].Y <= InRangeY.Y)
		{
			FVector2D BoundBottom(Vertices[i].X), BoundTop(Vertices[i].X);

			if (i != 0)
			{
				if (Vertices[i - 1].Y < InRangeY.X)
					BoundBottom = Vertices[i - 1] + (Vertices[i] - Vertices[i - 1]) * (InRangeY.X - Vertices[i - 1].Y) / (Vertices[i].Y - Vertices[i - 1].Y);
				else if (Vertices[i - 1].Y > InRangeY.Y)
					BoundTop = Vertices[i] + (Vertices[i - 1] - Vertices[i]) * (InRangeY.Y - Vertices[i].Y) / (Vertices[i - 1].Y - Vertices[i].Y);
			}
			if (i != Vertices.Num() - 1)
			{
				if (Vertices[i + 1].Y < InRangeY.X)
					BoundBottom = Vertices[i + 1] + (Vertices[i] - Vertices[i + 1]) * (InRangeY.X - Vertices[i + 1].Y) / (Vertices[i].Y - Vertices[i + 1].Y);
				else if (Vertices[i + 1].Y > InRangeY.Y)
					BoundTop = Vertices[i] + (Vertices[i + 1] - Vertices[i]) * (InRangeY.Y - Vertices[i].Y) / (Vertices[i + 1].Y - Vertices[i].Y);
			}
			float max3 = FMath::Max3(BoundBottom.X, Vertices[i].X, BoundTop.X);
			float min3 = FMath::Min3(BoundBottom.X, Vertices[i].X, BoundTop.X);

			if (max3 > MaxInRange.X) MaxInRange.X = max3;
			if (min3 < MinInRange.X) MinInRange.X = min3;
		}
	}
}