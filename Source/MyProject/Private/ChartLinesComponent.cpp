// Fill out your copyright notice in the Description page of Project Settings.

#include "ChartLinesComponent.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

DEFINE_LOG_CATEGORY(LogChartLines);

// Sets default values for this component's properties
UChartLinesComponent::UChartLinesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bTickInEditor = false;

	LinesControllerProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Lines Controller"));
	LinesControllerProcMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/M_ChartLine"));
	if (MaterialFinder.Succeeded())
		Material = (UMaterialInterface*)MaterialFinder.Object;
}

void UChartLinesComponent::CreateChartLine(int32 Index, const TArray<FVector2D>& InVertices)
{
	if (Index >= 0)
	{
		// Ensure lines array is long enough
		if (Index >= Lines.Num())
			Lines.SetNum(Index + 1, false);

		// Reset this line (in case it already existed)
		FLineSection& NewLine = Lines[Index];
		NewLine.Reset();
		NewLine.Vertices = InVertices;
		NewLine.Color = FLinearColor::MakeFromHSV8(Index * 20 + 100, 255, 255);

		Lines[Index].DynMaterial = LinesControllerProcMesh->CreateDynamicMaterialInstance(Index, Material);
		Lines[Index].DynMaterial->SetVectorParameterValue("LineColor", Lines[Index].Color);

		UpdateChartLine(Index);
	}
}

void UChartLinesComponent::ClearChartLine(int32 Index)
{
	if (Index < Lines.Num() && Index >= 0)
	{
		Lines[Index].Reset();
		LinesControllerProcMesh->ClearMeshSection(Index);
	}
}

void UChartLinesComponent::ClearAllChartLines()
{
	Lines.Reset();
	LinesControllerProcMesh->ClearAllMeshSections();
}

int32 UChartLinesComponent::GetNumChartLines() const
{
	return Lines.Num();
}

FLineSection* UChartLinesComponent::GetChartLineSection(int32 Index)
{
	if (Index < Lines.Num() && Index >= 0)
		return &Lines[Index];
	else
		return nullptr;
}

void UChartLinesComponent::SetChartLineSection(int32 Index, const FLineSection& Section)
{
	if (Index >= 0)
	{
		// Ensure lines array is long enough
		if (Index >= Lines.Num())
			Lines.SetNum(Index + 1, false);
		Lines[Index] = Section;

		UpdateChartLine(Index);
	}
}

void UChartLinesComponent::SetChartLineVertices(int32 Index, const TArray<FVector2D>& InVertices)
{
	if (Index < Lines.Num() && Index >= 0)
	{
		Lines[Index].Vertices = InVertices;

		if (Lines[Index].DynMaterial)
			UpdateChartLine(Index);
	}
}

void UChartLinesComponent::SetChartLineWidth(int32 Index, float Value)
{
	if (Index < Lines.Num() && Index >= 0 && Value > 0.0)
	{
		Lines[Index].Width = Value;

		if (Lines[Index].DynMaterial)
			UpdateChartLine(Index);
	}
}

void UChartLinesComponent::SetChartLineColor(int32 Index, FLinearColor Value)
{
	if (Index < Lines.Num() && Index >= 0)
	{
		Lines[Index].Color = Value;

		if (Lines[Index].DynMaterial)
			Lines[Index].DynMaterial->SetVectorParameterValue("LineColor", Lines[Index].Color);
	}
}

void UChartLinesComponent::SetChartLineType(int32 Index, EChartLineType Value)
{
	if (Index < Lines.Num() && Index >= 0)
	{
		Lines[Index].Type = Value;


		if (Lines[Index].DynMaterial)
			UpdateChartLine(Index);
	}
}

void UChartLinesComponent::SetChartLineDashLength(int32 Index, float Value)
{
	if (Index < Lines.Num() && Index >= 0 && Value > 0.0)
	{
		Lines[Index].DashLength = Value;

		if (Lines[Index].Type == EChartLineType::Dashed && Lines[Index].DynMaterial)
			UpdateChartLine(Index, true);
	}
}

void UChartLinesComponent::SetChartLineDashInterval(int32 Index, float Value)
{
	if (Index < Lines.Num() && Index >= 0 && Value > 0.0)
	{
		Lines[Index].DashInterval = Value;

		if (Lines[Index].Type == EChartLineType::Dashed && Lines[Index].DynMaterial)
			UpdateChartLine(Index, true);
	}
}

void UChartLinesComponent::TrimToChart(const TArray<FVector>& InPlanePositions, const TArray<FVector>& InPlaneNormals)
{
	if (InPlanePositions.Num() == InPlaneNormals.Num())
	{
		UProceduralMeshComponent* EmptyOutProcMesh;
		for (int i = 0; i < InPlanePositions.Num(); i++)
			UKismetProceduralMeshLibrary::SliceProceduralMesh(LinesControllerProcMesh, InPlanePositions[i], InPlaneNormals[i], false, EmptyOutProcMesh, EProcMeshSliceCapOption::NoCap, nullptr);
	}
	else UE_LOG(LogChartLines, Warning, TEXT("[%s] The number of plane positions (%d) does not match the number of normals (%d)."), TEXT("UChartLinesComponent::TrimToChart"), InPlanePositions.Num(), InPlaneNormals.Num());
}

void UChartLinesComponent::UpdateChartLine(int32 Index, bool bUpdate)
{
	Lines[Index].ZDepth = Index * Step;

	TArray<FVector> Vertices;
	TArray<FVector2D> UVs;

	Lines[Index].BuildLine(Vertices, UVs);
	Lines[Index].DynMaterial->SetScalarParameterValue("Width", Lines[Index].DashLength / (2 * (Lines[Index].DashInterval + Lines[Index].DashLength)));
	if (Lines[Index].Type == EChartLineType::Dotted)
		Lines[Index].DynMaterial->SetScalarParameterValue("bDotted", 1.0);
	else Lines[Index].DynMaterial->SetScalarParameterValue("bDotted", 0.0);

	if (Vertices.Num() > 2)
	{
		TArray<int32> Triangles;
		TArray<FVector> EmptyArrayNormals;
		TArray<FColor> EmptyColors;
		TArray<FProcMeshTangent> EmptyArrayTangents;

		if (bUpdate && Index < LinesControllerProcMesh->GetNumSections())
			LinesControllerProcMesh->UpdateMeshSection(Index, Vertices, EmptyArrayNormals, UVs, EmptyColors, EmptyArrayTangents);
		else
		{
			UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Vertices.Num() / 2, 2, true, Triangles);
			LinesControllerProcMesh->CreateMeshSection(Index, Vertices, Triangles, EmptyArrayNormals, UVs, EmptyColors, EmptyArrayTangents, false);
		}
	}
}