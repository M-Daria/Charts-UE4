// Fill out your copyright notice in the Description page of Project Settings.


#include "ChartBackgroundComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogChartBackground);


// Sets default values for this component's properties
UChartBackgroundComponent::UChartBackgroundComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/M_ChartBackground"));
	if (MaterialFinder.Succeeded())
		Material = (UMaterialInterface*)MaterialFinder.Object;

	BackgroundControllerProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Background Controller"));
	BackgroundControllerProcMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	for (int i = 0; i < MaxGridLinesCount; i++)
	{
		FName Name = *FString::Printf(TEXT("Axis Values Component %i"), i);
		AxisValues.Emplace(CreateDefaultSubobject<UTextRenderComponent>(Name));
		AxisValues[i]->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		AxisValues[i]->SetText(FText::FromString(""));
	}

	for (int i = 0; i < 3; i++)
	{
		FName Name = *FString::Printf(TEXT("Text Part Component %i"), i);
		TextParts.Emplace(CreateDefaultSubobject<UTextRenderComponent>(Name));
		TextParts[i]->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		TextParts[i]->SetText(FText::FromString(""));
	}

	BackgroundViewportSize = FVector2D(500.f);
	XRange = FVector2D(0.f, 1.f);
	YRange = FVector2D(0.f, 1.f);
	GridType = EChartGridType::Solid;
	BackgroundColor = FLinearColor::White;
	GridColor = FLinearColor::Gray;
}

void UChartBackgroundComponent::CreateChartBackground(const FVector2D& XRangeValue, const FVector2D& YRangeValue)
{
	if (XRangeValue.X < XRangeValue.Y)
		XRange = XRangeValue;
	if (YRangeValue.X < YRangeValue.Y)
		YRange = YRangeValue;

	TArray<int32> Triangles;
	TArray<FVector> Vertices;
	TArray<FVector> EmptyNormals;
	TArray<FVector2D> EmptyUVs;
	TArray<FColor> EmptyColors;
	TArray<FProcMeshTangent> EmptyTangents;

	Vertices.AddDefaulted(4);
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(2, 2, true, Triangles);
	BackgroundControllerProcMesh->CreateMeshSection(Background, Vertices, Triangles, EmptyNormals, EmptyUVs, EmptyColors, EmptyTangents, false);
	BackgroundControllerProcMesh->CreateMeshSection(Grid, Vertices, Triangles, EmptyNormals, EmptyUVs, EmptyColors, EmptyTangents, false);
	/*Vertices.AddDefaulted(6);
	Triangles.Reset();
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(5, 2, false, Triangles);
	BackgroundControllerProcMesh->CreateMeshSection(Frame, Vertices, Triangles, EmptyNormals, EmptyUVs, EmptyColors, EmptyTangents, false);*/

	if (DynMaterials.Num() == 0)
	{
		DynMaterials.Add(BackgroundControllerProcMesh->CreateDynamicMaterialInstance(Background, Material));
		DynMaterials.Add(BackgroundControllerProcMesh->CreateDynamicMaterialInstance(Grid, Material));
		//DynMaterials.Add(BackgroundControllerProcMesh->CreateDynamicMaterialInstance(Frame, Material));
	}

	DynMaterials[Background]->SetVectorParameterValue("BackgroundColor", BackgroundColor);
	DynMaterials[Background]->SetVectorParameterValue("GridColor", GridColor);
	DynMaterials[Grid]->SetVectorParameterValue("GridColor", GridColor);
	//DynMaterials[Frame]->SetVectorParameterValue("BackgroundColor", FrameColor);

	DrawChartBackground();
}

void UChartBackgroundComponent::SetChartBackgroundXRange(const FVector2D& XRangeValue)
{
	if (XRangeValue.X > XRangeValue.Y)
	{
		UE_LOG(LogChartBackground, Warning, TEXT("[%s] Invalid x range."), TEXT("UChartBackgroundComponent::SetChartBackgroundXRange"));

		XRange = FVector2D(0.f, 1.f);
	}

	XRange = XRangeValue;

	if (BackgroundControllerProcMesh->GetNumSections() > 0)
		DrawChartBackground();
}

void UChartBackgroundComponent::SetChartBackgroundYRange(const FVector2D& YRangeValue)
{
	if (YRangeValue.X > YRangeValue.Y)
	{
		UE_LOG(LogChartBackground, Warning, TEXT("[%s] Invalid y range."), TEXT("UChartBackgroundComponent::SetChartBackgroundYRange"));

		YRange = FVector2D(0.f, 1.f);
	}

	YRange = YRangeValue;

	if (BackgroundControllerProcMesh->GetNumSections() > 0)
		DrawChartBackground();
}

void UChartBackgroundComponent::SetChartBackgroundViewportSize(const FVector2D& ViewportSize)
{
	if (ViewportSize.X >= 10 && ViewportSize.Y >= 10)
	{
		BackgroundViewportSize = ViewportSize;

		if (BackgroundControllerProcMesh->GetNumSections() > 0)
			DrawChartBackground();
	}
}

void UChartBackgroundComponent::SetChartGridType(EChartGridType Type)
{
	if (GridType != Type)
	{
		GridType = Type;

		if (BackgroundControllerProcMesh->GetNumSections() > 0)
			DrawChartBackground();
	}
}

void UChartBackgroundComponent::SetChartBackgroundColor(const FLinearColor& Color)
{
	BackgroundColor = Color;

	if (BackgroundControllerProcMesh->GetNumSections() > 0)
		DynMaterials[Background]->SetVectorParameterValue("BackgroundColor", BackgroundColor);
}

void UChartBackgroundComponent::SetChartGridColor(const FLinearColor& Color)
{
	GridColor = Color;

	if (BackgroundControllerProcMesh->GetNumSections() > 0)
	{
		DynMaterials[Background]->SetVectorParameterValue("GridColor", GridColor);
		DynMaterials[Grid]->SetVectorParameterValue("GridColor", GridColor);
	}
}

void UChartBackgroundComponent::SetTextColor(EChartTextNames Type, const FColor& Color)
{
	if (Type == EChartTextNames::AxisValues)
		for (auto& TextValue : AxisValues)
			TextValue->SetTextRenderColor(Color);
	else
		TextParts[(uint8)Type]->SetTextRenderColor(Color);
}

void UChartBackgroundComponent::SetTextSize(EChartTextNames Type, float Value)
{
	if (BackgroundControllerProcMesh->GetNumSections() > 0)
	{
		if (Type == EChartTextNames::AxisValues)
			for (auto& TextValue : AxisValues)
				TextValue->SetWorldSize(Value);
		else
			TextParts[(uint8)Type]->SetWorldSize(Value);
	}
}

void UChartBackgroundComponent::SetTextTitle(const FText& Value)
{
	if (BackgroundControllerProcMesh->GetNumSections() > 0)
		TextParts[(uint8)EChartTextNames::Title]->SetText(Value);
	else UE_LOG(LogChartBackground, Warning, TEXT("[%s] Chart has not been created. Changes were not applied."), TEXT("UChartBackgroundComponent::SetChartTitle"));
}

void UChartBackgroundComponent::SetTextXLabel(const FText& Value)
{
	if (BackgroundControllerProcMesh->GetNumSections() > 0)
		TextParts[(uint8)EChartTextNames::XLabel]->SetText(Value);
	else UE_LOG(LogChartBackground, Warning, TEXT("[%s] Chart has not been created. Changes were not applied."), TEXT("UChartBackgroundComponent::SetChartXLabel"));
}

void UChartBackgroundComponent::SetTextYLabel(const FText& Value)
{
	if (BackgroundControllerProcMesh->GetNumSections() > 0)
		TextParts[(uint8)EChartTextNames::YLabel]->SetText(Value);
	else UE_LOG(LogChartBackground, Warning, TEXT("[%s] Chart has not been created. Changes were not applied."), TEXT("UChartBackgroundComponent::SetChartYLabel"));
}


void UChartBackgroundComponent::DrawChartBackground()
{
	TArray< TArray<FVector2D> > UVs;
	UVs.AddDefaulted();

	float StepX = GetGridStep(XRange), StepY = GetGridStep(YRange);
	float StepXClamped = FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, XRange.Y - XRange.X), FVector2D(0.f, BackgroundViewportSize.X), StepX);
	float StepYClamped = FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, YRange.Y - YRange.X), FVector2D(0.f, BackgroundViewportSize.Y), StepY);
	StepClamped = FVector2D(StepXClamped, StepYClamped);
	float OffsetX = GetGridOffset(XRange.X, StepX), OffsetY = GetGridOffset(YRange.X, StepY);
	float OffsetXClamped = FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, XRange.Y - XRange.X), FVector2D(0.f, BackgroundViewportSize.X), OffsetX);
	float OffsetYClamped = FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, YRange.Y - YRange.X), FVector2D(0.f, BackgroundViewportSize.Y), OffsetY);

	float GridWidth = BackgroundViewportSize.GetMin() * GridLineWidth;

	switch (GridType)
	{
	case EChartGridType::NoGrid:
		UVs[Background].AddDefaulted(4);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(0.0, 0.0), FVector2D(0.0, 0.0));
		break;

	case EChartGridType::Solid:
		UVs.AddDefaulted();
		BuildSolidUVs(UVs[Background], UVs[Grid]);
		DrawBackgroundPart(Background, UVs[Background]);
		DrawBackgroundPart(Grid, UVs[Grid]);

		// Horizontal grid
		SetMaterialGridParams(Background, FVector2D(GridWidth / StepClamped.Y, GridBandLength - (OffsetYClamped / StepClamped.Y)), FVector2D(GridBandLength, 0.0));
		// Vertical grid
		SetMaterialGridParams(Grid, FVector2D(GridBandLength, 0.0), FVector2D(GridWidth / StepClamped.X, GridBandLength - (OffsetXClamped / StepClamped.X)), 1.0);
		break;

	case EChartGridType::SolidHorizontal:
		BuildSolidUVs(UVs[Background], true);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(GridWidth / StepClamped.Y, GridBandLength - (OffsetYClamped / StepClamped.Y)), FVector2D(GridBandLength, 0.0));
		break;

	case EChartGridType::SolidVertical:
		BuildSolidUVs(UVs[Background]);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(GridBandLength, 0.0), FVector2D(GridWidth / StepClamped.X, GridBandLength - (OffsetXClamped / StepClamped.X)));
		break;

	case EChartGridType::Dashed:
		UVs.AddZeroed();
		BuildDashedUVs(UVs[Background], UVs[Grid]);
		DrawBackgroundPart(Background, UVs[Background]);
		DrawBackgroundPart(Grid, UVs[Grid]);

		SetMaterialGridParams(Background, FVector2D(GridWidth / StepClamped.Y, GridBandLength - (OffsetYClamped / StepClamped.Y)), FVector2D(GridBandLength - GridDashInterval, GridDashInterval));
		SetMaterialGridParams(Grid, FVector2D(GridBandLength - GridDashInterval, GridDashInterval), FVector2D(GridWidth / StepClamped.X, GridBandLength - (OffsetXClamped / StepClamped.X)), 1.0);
		break;

	case EChartGridType::DashedHorizontal:
		BuildDashedUVs(UVs[Background], true);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(GridWidth / StepClamped.Y, GridBandLength - (OffsetYClamped / StepClamped.Y)), FVector2D(GridBandLength - GridDashInterval, GridDashInterval));
		break;

	case EChartGridType::DashedVertical:
		BuildDashedUVs(UVs[Background]);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(GridBandLength - GridDashInterval, GridDashInterval), FVector2D(GridWidth / StepClamped.X, GridBandLength - (OffsetXClamped / StepClamped.X)));
		break;

	case EChartGridType::StripedHorizontal:
		BuildStripedUVs(UVs[Background], true);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(GridBandLength / 2, OffsetYClamped * 2 / StepClamped.Y), FVector2D(GridBandLength, 0.0));
		break;

	case EChartGridType::StripedVertical:
		BuildStripedUVs(UVs[Background]);
		DrawBackgroundPart(Background, UVs[Background]);
		SetMaterialGridParams(Background, FVector2D(GridBandLength, 0.0), FVector2D(GridBandLength / 2, OffsetXClamped * 2 / StepClamped.X));
		break;
	}

	//UVs.AddDefaulted();
	//DrawBackgroundPart(Frame, UVs.Last());

	RenderAxisValuesText(FVector2D(OffsetXClamped, OffsetYClamped), FVector2D(OffsetX, OffsetY), FVector2D(StepX, StepY));
}

void UChartBackgroundComponent::DrawBackgroundPart(EBackgroundPartNames Part, const TArray<FVector2D>& UVs)
{
	TArray<FVector> Vertices;
	TArray<FVector> EmptyNormals;
	TArray<FColor> EmptyColors;
	TArray<FProcMeshTangent> EmptyTangents;

	switch (Part)
	{
	case Background:
		Vertices.Add(FVector(0.f, DistBackgroundParts * Background, 0.0));
		Vertices.Add(FVector(0.f, DistBackgroundParts * Background, BackgroundViewportSize.Y));
		Vertices.Add(FVector(BackgroundViewportSize.X, DistBackgroundParts * Background, 0.f));
		Vertices.Add(FVector(BackgroundViewportSize.X, DistBackgroundParts * Background, BackgroundViewportSize.Y));

		BackgroundControllerProcMesh->UpdateMeshSection(Background, Vertices, EmptyNormals, UVs, EmptyColors, EmptyTangents);
		BackgroundControllerProcMesh->SetMeshSectionVisible(Grid, false);
		break;

	case Grid:
		Vertices.Add(FVector(0.f, DistBackgroundParts * Grid, 0.f));
		Vertices.Add(FVector(0.f, DistBackgroundParts * Grid, BackgroundViewportSize.Y));
		Vertices.Add(FVector(BackgroundViewportSize.X, DistBackgroundParts * Grid, 0.f));
		Vertices.Add(FVector(BackgroundViewportSize.X, DistBackgroundParts * Grid, BackgroundViewportSize.Y));

		BackgroundControllerProcMesh->UpdateMeshSection(Grid, Vertices, EmptyNormals, UVs, EmptyColors, EmptyTangents);
		BackgroundControllerProcMesh->SetMeshSectionVisible(Grid, true);
		break;

	case Frame:
		/*Vertices.Add(FVector(-FrameWidth, DistBackgroundParts * Frame, -FrameWidth));
		Vertices.Add(FVector(FrameWidth, DistBackgroundParts * Frame, FrameWidth));
		Vertices.Add(FVector(-FrameWidth, DistBackgroundParts * Frame, BackgroundViewportSize.Y + FrameWidth));
		Vertices.Add(FVector(FrameWidth, DistBackgroundParts * Frame, BackgroundViewportSize.Y - FrameWidth));
		Vertices.Add(FVector(BackgroundViewportSize.X + FrameWidth, DistBackgroundParts * Frame, BackgroundViewportSize.Y + FrameWidth));
		Vertices.Add(FVector(BackgroundViewportSize.X - FrameWidth, DistBackgroundParts * Frame, BackgroundViewportSize.Y - FrameWidth));
		Vertices.Add(FVector(BackgroundViewportSize.X + FrameWidth, DistBackgroundParts * Frame, -FrameWidth));
		Vertices.Add(FVector(BackgroundViewportSize.X - FrameWidth, DistBackgroundParts * Frame, FrameWidth));
		Vertices.Add(FVector(-FrameWidth, DistBackgroundParts * Frame, -FrameWidth));
		Vertices.Add(FVector(FrameWidth, DistBackgroundParts * Frame, FrameWidth));

		BackgroundControllerProcMesh->UpdateMeshSection(Frame, Vertices, EmptyNormals, UVs, EmptyColors, EmptyTangents);*/
		break;
	}
}

void UChartBackgroundComponent::RenderAxisValuesText(const FVector2D& StartClamped, const FVector2D& Start, const FVector2D& Step)
{
	FVector2D Current = Start + FVector2D(XRange.X, YRange.X);
	FVector2D CurrentClamped = StartClamped;
	float MaxXSize = 0, MaxYSize = 0;
	bool bRotated = false;
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.MaximumFractionalDigits = MaximumFractionalDigits;

	for (int i = 0; i < MaxGridLinesCount / 2; i++, Current += Step, CurrentClamped += StepClamped)
	{
		// X-Axis values
		if (Current.X > XRange.Y)
			AxisValues[i]->SetText(FText::FromString(""));
		else
		{
			AxisValues[i]->SetText(FText::AsNumber(Current.X, &NumberFormatOptions));
			AxisValues[i]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
			AxisValues[i]->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
			AxisValues[i]->SetRelativeLocation(FVector(CurrentClamped.X, 0.0, -TextOffset));
			AxisValues[i]->SetRelativeRotation(FRotator(0, 90, 0));

			if (AxisValues[i]->GetTextWorldSize().X >= StepClamped.X)
				bRotated = true;
			MaxYSize = MaxYSize < AxisValues[i]->GetTextWorldSize().Z ? AxisValues[i]->GetTextWorldSize().Z : MaxYSize;
		}

		// Y-Axis values
		if (Current.Y > YRange.Y)
			AxisValues[MaxGridLinesCount / 2 + i]->SetText(FText::FromString(""));
		else
		{
			AxisValues[MaxGridLinesCount / 2 + i]->SetText(FText::AsNumber(Current.Y, &NumberFormatOptions));
			AxisValues[MaxGridLinesCount / 2 + i]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Right);
			AxisValues[MaxGridLinesCount / 2 + i]->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
			AxisValues[MaxGridLinesCount / 2 + i]->SetRelativeLocation(FVector(-TextOffset, 0.0, CurrentClamped.Y));
			AxisValues[MaxGridLinesCount / 2 + i]->SetRelativeRotation(FRotator(0, 90, 0));

			MaxXSize = MaxXSize < AxisValues[MaxGridLinesCount / 2 + i]->GetTextWorldSize().X ? AxisValues[MaxGridLinesCount / 2 + i]->GetTextWorldSize().X : MaxXSize;
		}
	}
	if (bRotated)
		for (int i = 0; i < MaxGridLinesCount / 2; i++)
		{
			AxisValues[i]->AddRelativeRotation(FRotator(0, 0, -45));
			AxisValues[i]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
			MaxYSize = MaxYSize < AxisValues[i]->GetTextWorldSize().Z ? AxisValues[i]->GetTextWorldSize().Z : MaxYSize;
		}

	TextParts[(uint8)EChartTextNames::Title]->SetRelativeLocation(FVector(BackgroundViewportSize.X / 2, 0.0, BackgroundViewportSize.Y + TextOffset));
	TextParts[(uint8)EChartTextNames::Title]->SetRelativeRotation(FRotator(0, 90, 0));
	TextParts[(uint8)EChartTextNames::Title]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);

	TextParts[(uint8)EChartTextNames::XLabel]->SetRelativeLocation(FVector(BackgroundViewportSize.X / 2, 0.0, -MaxYSize - 2*TextOffset));
	TextParts[(uint8)EChartTextNames::XLabel]->SetRelativeRotation(FRotator(0, 90, 0));
	TextParts[(uint8)EChartTextNames::XLabel]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextParts[(uint8)EChartTextNames::XLabel]->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);

	TextParts[(uint8)EChartTextNames::YLabel]->SetRelativeLocation(FVector(-MaxXSize - 2*TextOffset, 0.0, BackgroundViewportSize.Y / 2));
	TextParts[(uint8)EChartTextNames::YLabel]->SetRelativeRotation(FRotator(0, 90, 90));
	TextParts[(uint8)EChartTextNames::YLabel]->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
}

void UChartBackgroundComponent::SetMaterialGridParams(EBackgroundPartNames Part, const FVector2D& VertWidthOffset, const FVector2D& HorWidthOffset, float bSecondGridLayer)
{
	if (DynMaterials.IsValidIndex(Part))
	{
		DynMaterials[Part]->SetScalarParameterValue("WidthVert", VertWidthOffset.X);
		DynMaterials[Part]->SetScalarParameterValue("OffsetVert", VertWidthOffset.Y);
		DynMaterials[Part]->SetScalarParameterValue("WidthHor", HorWidthOffset.X);
		DynMaterials[Part]->SetScalarParameterValue("OffsetHor", HorWidthOffset.Y);
		DynMaterials[Part]->SetScalarParameterValue("bSecondGridLayer", bSecondGridLayer);
	}
}

void UChartBackgroundComponent::BuildSolidUVs(TArray<FVector2D>& UV0s, TArray<FVector2D>& UV1s) const
{
	UV0s.Empty(); UV1s.Empty();

	// Horizontal grid
	UV0s.Add(FVector2D(0.f));
	UV0s.Add(FVector2D(0.f, BackgroundViewportSize.Y / StepClamped.Y));
	UV0s.Add(FVector2D(1.f, 0.f));
	UV0s.Add(FVector2D(1.f, BackgroundViewportSize.Y / StepClamped.Y));

	// Vertical grid
	UV1s.Add(FVector2D(0.f));
	UV1s.Add(FVector2D(0.f, 1.f));
	UV1s.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, 0.f));
	UV1s.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, 1.f));
}

void UChartBackgroundComponent::BuildSolidUVs(TArray<FVector2D>& UVs, bool bHorisontal) const
{
	UVs.Empty();

	if (bHorisontal)
	{
		UVs.Add(FVector2D(0.f));
		UVs.Add(FVector2D(0.f, BackgroundViewportSize.Y / StepClamped.Y));
		UVs.Add(FVector2D(1.f, 0.f));
		UVs.Add(FVector2D(1.f, BackgroundViewportSize.Y / StepClamped.Y));
	}
	else
	{
		UVs.Add(FVector2D(0.f));
		UVs.Add(FVector2D(0.f, 1.f));
		UVs.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, 0.f));
		UVs.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, 1.f));
	}
}

void UChartBackgroundComponent::BuildDashedUVs(TArray<FVector2D>& UV0s, TArray<FVector2D>& UV1s) const
{
	UV0s.Empty(); UV1s.Empty();
	float DashLength = BackgroundViewportSize.GetMin() * GridDashLength;
	float Decimal = (GridBandLength - GridDashInterval) * 2;

	UV0s.Add(FVector2D(0.f));
	UV0s.Add(FVector2D(0.f, BackgroundViewportSize.Y / StepClamped.Y));
	UV0s.Add(FVector2D(AttachToDecimal(BackgroundViewportSize.X / DashLength, Decimal), 0.f));
	UV0s.Add(FVector2D(AttachToDecimal(BackgroundViewportSize.X / DashLength, Decimal), BackgroundViewportSize.Y / StepClamped.Y));

	UV1s.Add(FVector2D(0.f));
	UV1s.Add(FVector2D(0.f, AttachToDecimal(BackgroundViewportSize.Y / DashLength, Decimal)));
	UV1s.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, 0.f));
	UV1s.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, AttachToDecimal(BackgroundViewportSize.Y / DashLength, Decimal)));
}

void UChartBackgroundComponent::BuildDashedUVs(TArray<FVector2D>& UVs, bool bHorisontal) const
{
	UVs.Empty();
	float DashLength = BackgroundViewportSize.GetMin() * GridDashLength;
	float Decimal = (GridBandLength - GridDashInterval) * 2;

	if (bHorisontal)
	{
		UVs.Add(FVector2D(0.f));
		UVs.Add(FVector2D(0.f, BackgroundViewportSize.Y / StepClamped.Y));
		UVs.Add(FVector2D(AttachToDecimal(BackgroundViewportSize.X / DashLength, Decimal), 0.f));
		UVs.Add(FVector2D(AttachToDecimal(BackgroundViewportSize.X / DashLength, Decimal), BackgroundViewportSize.Y / StepClamped.Y));
	}
	else
	{
		UVs.Add(FVector2D(0.f));
		UVs.Add(FVector2D(0.f, AttachToDecimal(BackgroundViewportSize.Y / DashLength, Decimal)));
		UVs.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, 0.f));
		UVs.Add(FVector2D(BackgroundViewportSize.X / StepClamped.X, AttachToDecimal(BackgroundViewportSize.Y / DashLength, Decimal)));
	}
}

void UChartBackgroundComponent::BuildStripedUVs(TArray<FVector2D>& UVs, bool bHorisontal) const
{
	UVs.Empty();

	if (bHorisontal)
	{
		UVs.Add(FVector2D(0.f));
		UVs.Add(FVector2D(0.f, BackgroundViewportSize.Y * 0.5 / StepClamped.Y));
		UVs.Add(FVector2D(1.f, 0.f));
		UVs.Add(FVector2D(1.f, BackgroundViewportSize.Y * 0.5 / StepClamped.Y));
	}
	else
	{
		UVs.Add(FVector2D(0.f));
		UVs.Add(FVector2D(0.f, 1.0));
		UVs.Add(FVector2D(BackgroundViewportSize.X * 0.5 / StepClamped.X, 0.f));
		UVs.Add(FVector2D(BackgroundViewportSize.X * 0.5 / StepClamped.X, 1.f));
	}
}

float UChartBackgroundComponent::GetGridStep(const FVector2D& Range) const
{
	float Dist = Range.Y - Range.X;
	FString Left, Right;
	float Pow, Step;
	float Steps[4] = { FMath::FloorToFloat(MaxGridLinesCount / 16), FMath::FloorToFloat(MaxGridLinesCount / 8), FMath::FloorToFloat(MaxGridLinesCount / 4), FMath::FloorToFloat(MaxGridLinesCount / 2) - 1 };
	
	FString::SanitizeFloat(Dist).Split(".", &Left, &Right);

	if (Dist > 0.f && Dist < 1.f)
		Pow = -(Right.Len() - FString::SanitizeFloat(FCString::Atof(*Right)).Len() + 2);
	else
		Pow = Left.Len();

	float NormalizedDist = UKismetMathLibrary::NormalizeToRange(Dist, 0.f, pow(10., Pow - 1.f));

	if (NormalizedDist < Steps[0]) Step = Steps[0] / 10.f;
	else if (NormalizedDist <= Steps[1]) Step = Steps[1] / 10.f;
	else if (NormalizedDist <= Steps[2]) Step = Steps[2] / 10.f;
	else Step = Steps[3] / 10.f;

	Step = Step * pow(10., Pow - 1.f);

	return Step;
}

float UChartBackgroundComponent::GetGridOffset(float Start, float Step) const
{
	float Offset = FMath::GridSnap(Start, Step);

	if (Offset < Start) Offset += Step;

	return Offset - Start;
}

float UChartBackgroundComponent::AttachToDecimal(float Value, float Decimal) const
{
	float Var1 = FMath::FloorToFloat(Value) + Decimal;
	float Var2 = FMath::CeilToFloat(Value) + Decimal;

	if (Value - Var1 > Value - Var2) return Var2;
	return Var1;
}