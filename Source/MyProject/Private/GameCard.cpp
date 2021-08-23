// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCard.h"
#include "ProceduralMeshComponent.h"
#include "ChartViewComponent.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/TextRenderComponent.h"

AGameCard::AGameCard()
{
	PrimaryActorTick.bCanEverTick = false; 
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/M_Gradient"));
	if (MaterialFinder.Succeeded())
		BackgroundMaterial = (UMaterialInterface*)MaterialFinder.Object;

	Background = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Background"));
	BackgroundCharts = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("BackgroundCharts"));
	Icons = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Icons"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	Background->SetupAttachment(RootComponent);
	BackgroundCharts->SetupAttachment(RootComponent);
	Icons->SetupAttachment(RootComponent);

	TArray<int32> Triangles = { 2, 1, 0, 1, 2, 3 };
	TArray<FVector> Vertices;
	TArray<FVector> EmptyNormals;
	TArray<FVector2D> EmptyUVs;
	TArray<FColor> EmptyColors;
	TArray<FProcMeshTangent> EmptyTangents;

	Vertices.AddDefaulted(4);
	Background->CreateMeshSection(0, Vertices, Triangles, EmptyNormals, EmptyUVs, EmptyColors, EmptyTangents, false);

	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder0(TEXT("/Game/Materials/Material_Instances/MI_Facebook"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder0.Object);
		BrandNames.Emplace(FText::FromString("Facebook"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder1(TEXT("/Game/Materials/Material_Instances/MI_AMD"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder1.Object);
		BrandNames.Emplace(FText::FromString("AMD"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder2(TEXT("/Game/Materials/Material_Instances/MI_Samsung"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder2.Object);
		BrandNames.Emplace(FText::FromString("Samsung"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder3(TEXT("/Game/Materials/Material_Instances/MI_Apple"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder3.Object);
		BrandNames.Emplace(FText::FromString("Apple"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder4(TEXT("/Game/Materials/Material_Instances/MI_EpicGames"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder4.Object);
		BrandNames.Emplace(FText::FromString("EpicGames"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder5(TEXT("/Game/Materials/Material_Instances/MI_Netflix"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder5.Object);
		BrandNames.Emplace(FText::FromString("Netflix"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder6(TEXT("/Game/Materials/Material_Instances/MI_Nvidia"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder6.Object);
		BrandNames.Emplace(FText::FromString("Nvidia"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder7(TEXT("/Game/Materials/Material_Instances/MI_Microsoft"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder7.Object);
		BrandNames.Emplace(FText::FromString("Microsoft"));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> IconMaterialFinder8(TEXT("/Game/Materials/Material_Instances/MI_Intel"));
	if (MaterialFinder.Succeeded())
	{
		IconMaterials.Emplace((UMaterialInterface*)IconMaterialFinder8.Object);
		BrandNames.Emplace(FText::FromString("Intel"));
	}
}

void AGameCard::BeginPlay()
{
	Super::BeginPlay();

	TArray<FVector2D> RandomVertices;
	for (int32 i = 0; i < 26; ++i)
		RandomVertices.Emplace(FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, 25.f), FVector2D(0.f, 150.f), i), FMath::FRandRange(0.f, 15.f));

	for (int32 i = 0; i < 9; ++i)
		AddChart(i, IconMaterials[i], BrandNames[i], 150.f, RandomVertices);

	DeleteChart(4);

	AddChart(10, IconMaterials[4], BrandNames[4], 150.f, RandomVertices);

	FChartParams tmp;
	GetChart(1, tmp);
	tmp.Chart->SetTextColor(EChartTextNames::AxisValues, FColor::Cyan);
	tmp.Price->SetText("$$$");
}

void AGameCard::AddChart(int32 Index, UMaterialInterface* Material, const FText& BrandName, float Price, const TArray<FVector2D>& Points)
{
	if (Index >= 0)
	{
		if (Index >= Charts.Num())
			Charts.SetNum(Index + 1, false);
		FChartParams& NewChart = Charts[Index];
		NewChart.Reset();

		NewChart.IconMaterial = Material;

		TArray<FVector> Vertices;
		TArray<int32> Triangles = { 2, 1, 0, 1, 2, 3 };
		TArray<FVector> EmptyNormals;
		TArray<FVector2D> UVs;
		TArray<FColor> EmptyColors;
		TArray<FProcMeshTangent> EmptyTangents;

		//BottomLeftIcon
		Vertices.Emplace(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X + IconOffsetFromTopLeft.X, 0.02f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) - IconSize.Y - IconOffsetFromTopLeft.Y);
		UVs.Emplace(0.f, 1.f);
		//TopLeftIcon
		Vertices.Emplace(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X + IconOffsetFromTopLeft.X, 0.02f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) - IconOffsetFromTopLeft.Y);
		UVs.Emplace(0.f, 0.f);
		//BottomRightIcon
		Vertices.Emplace(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X + IconOffsetFromTopLeft.X + IconSize.X, 0.02f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) - IconSize.Y - IconOffsetFromTopLeft.Y);
		UVs.Emplace(1.f, 1.f);
		//TopRightIcon
		Vertices.Emplace(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X + IconOffsetFromTopLeft.X + IconSize.X, 0.02f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) - IconOffsetFromTopLeft.Y);
		UVs.Emplace(1.f, 0.f);

		Icons->CreateMeshSection(Index, Vertices, Triangles, EmptyNormals, UVs, EmptyColors, EmptyTangents, false);
		Icons->CreateDynamicMaterialInstance(Index, Material);


		Vertices.Empty();
		UVs.Empty();

		//BottomLeftBackgroundChart
		Vertices.Emplace(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X, 0.01f, Index / ChartsInLine * BackgroundChartSize.Y + (Index / ChartsInLine + 1) * OffsetsBetweenCharts.Y);
		//TopLeftBackgroundChart
		Vertices.Emplace(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X, 0.01f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y));
		//BottomRightBackgroundChart
		Vertices.Emplace((Index % ChartsInLine + 1) * (OffsetsBetweenCharts.X + BackgroundChartSize.X), 0.01f, Index / ChartsInLine * BackgroundChartSize.Y + (Index / ChartsInLine + 1) * OffsetsBetweenCharts.Y);
		//TopRightBackgroundChart
		Vertices.Emplace((Index % ChartsInLine + 1) * (OffsetsBetweenCharts.X + BackgroundChartSize.X), 0.01f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y));

		BackgroundCharts->CreateMeshSection(Index, Vertices, Triangles, EmptyNormals, UVs, EmptyColors, EmptyTangents, false);
		BackgroundCharts->CreateDynamicMaterialInstance(Index, BackgroundMaterial);


		NewChart.Chart = NewObject<UChartViewComponent>(this);
		if (NewChart.Chart)
		{
			NewChart.Chart->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			NewChart.Chart->RegisterComponent();

			TArray<UObject*> ActorChildren;
			GetObjectsWithOuter(this, ActorChildren, true, RF_NoFlags, EInternalObjectFlags::PendingKill);
			for (UObject* Child : ActorChildren)
				if (UActorComponent* ChildComponent = Cast<UActorComponent>(Child))
					if (ChildComponent->bAutoRegister && !ChildComponent->IsRegistered())
						ChildComponent->RegisterComponent();
		}
		NewChart.Chart->SetRelativeLocation(FVector((Index % ChartsInLine + 1) * (OffsetsBetweenCharts.X + BackgroundChartSize.X) - ChartSize.X - ChartOffsetFromBottomRight.X, 0.02f, Index / ChartsInLine * BackgroundChartSize.Y + (Index / ChartsInLine + 1) * OffsetsBetweenCharts.Y + ChartOffsetFromBottomRight.Y));
		NewChart.Chart->CreateChartViewport();
		NewChart.Chart->SetViewportSize(ChartSize);
		NewChart.Chart->SetYAutoscale(false);
		NewChart.Chart->SetYRange(FVector2D(0.f, 15.f));
		NewChart.Chart->AddLine(Index, Points);
		//NewChart.Chart->SetLineWidth(Index, 2.f);
		NewChart.Chart->SetChartBackgroundColor(ChartBackgroundColor);
		NewChart.Chart->SetChartGridType(EChartGridType::StripedVertical);
		NewChart.Chart->SetChartGridColor(ChartGridColor);
		NewChart.Chart->SetTextTitle(BrandName);
		NewChart.Chart->SetTextSize(EChartTextNames::Title, 50);
		NewChart.Chart->SetTextColor(EChartTextNames::Title, TextBrandNameColor.ToFColor(true));
		NewChart.Chart->SetTextColor(EChartTextNames::AxisValues, TextAxisValuesColor.ToFColor(true));


		FNumberFormattingOptions NumberFormatOptions;
		NumberFormatOptions.AlwaysSign = true;
		NumberFormatOptions.UseGrouping = false;
		NumberFormatOptions.MaximumFractionalDigits = 2;
		NumberFormatOptions.MinimumFractionalDigits = 2;

		NewChart.Price = NewObject<UTextRenderComponent>(this);
		if (NewChart.Price)
		{
			NewChart.Price->RegisterComponent();
			NewChart.Price->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
		NewChart.Price->SetText(FText::AsCurrency(Price, TEXT("USD"), &NumberFormatOptions));
		NewChart.Price->SetTextRenderColor(TextPriceColor.ToFColor(true));
		NewChart.Price->SetRelativeLocation(FVector(Index % ChartsInLine * BackgroundChartSize.X + (Index % ChartsInLine + 1) * OffsetsBetweenCharts.X + IconOffsetFromTopLeft.X + IconSize.X / 2.f, 0.02f, (Index / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) - IconSize.Y - IconOffsetFromTopLeft.Y));
		NewChart.Price->SetRelativeRotation(FRotator(0, 90, 0));
		NewChart.Price->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		NewChart.Price->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);


		UpdateBackground();
	}
}

void AGameCard::GetChart(int32 Index, FChartParams& OutChart)
{
	OutChart.Reset();
	if (Index < Charts.Num())
		if (Charts[Index].Chart)
			OutChart = Charts[Index];
}

void AGameCard::DeleteChart(int32 Index)
{
	if (Index < Charts.Num())
	{
		Charts[Index].Price->ConditionalBeginDestroy();
		Charts[Index].Price = nullptr;

		TArray<UObject*> ActorChildren;
		GetObjectsWithOuter(Charts[Index].Chart, ActorChildren, true, RF_NoFlags, EInternalObjectFlags::PendingKill);
		for (UObject* Child : ActorChildren)
		{
			Child->ConditionalBeginDestroy();
			Child = nullptr;
		}

		BackgroundCharts->ClearMeshSection(Index);
		Icons->ClearMeshSection(Index);

		UpdateBackground();
	}
}

void AGameCard::DeleteAll()
{
	TArray<FVector> Vertices;
	TArray<FVector> EmptyNormals;
	TArray<FVector2D> EmptyUVs;
	TArray<FColor> EmptyColors;
	TArray<FProcMeshTangent> EmptyTangents;
	Vertices.AddDefaulted(4);
	Background->UpdateMeshSection(0, Vertices, EmptyNormals, EmptyUVs, EmptyColors, EmptyTangents);

	BackgroundCharts->ClearAllMeshSections();
	Icons->ClearAllMeshSections();

	for (FChartParams& Chart : Charts)
	{
		Chart.Price->ConditionalBeginDestroy();
		Chart.Price = nullptr;

		TArray<UObject*> ActorChildren;
		GetObjectsWithOuter(Chart.Chart, ActorChildren, true, RF_NoFlags, EInternalObjectFlags::PendingKill);
		for (UObject* Child : ActorChildren)
		{
			Child->ConditionalBeginDestroy();
			Child = nullptr;
		}
	}
	Charts.Empty();
}

void AGameCard::UpdateBackground()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles = { 2, 1, 0, 1, 2, 3 };
	TArray<FVector> EmptyNormals;
	TArray<FVector2D> UVs;
	TArray<FColor> EmptyColors;
	TArray<FProcMeshTangent> EmptyTangents;

	//BottomLeftBackgroundChart
	Vertices.Emplace(0.f, 0.f, 0.f);
	UVs.Emplace(1.f, 0.f);
	//TopLeftBackgroundChart
	Vertices.Emplace(0.f, 0.f, ((BackgroundCharts->GetNumSections() - 1) / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) + OffsetsBetweenCharts.Y);
	UVs.Emplace(1.f, 1.f);
	//BottomRightBackgroundChart
	Vertices.Emplace(FMath::Clamp(BackgroundCharts->GetNumSections(), 1, 3) * (OffsetsBetweenCharts.X + BackgroundChartSize.X) + OffsetsBetweenCharts.X, 0.f, 0.f);
	UVs.Emplace(0.f, 0.f);
	//TopRightBackgroundChart
	Vertices.Emplace(FMath::Clamp(BackgroundCharts->GetNumSections(), 1, 3) * (OffsetsBetweenCharts.X + BackgroundChartSize.X) + OffsetsBetweenCharts.X, 0.f, ((BackgroundCharts->GetNumSections() - 1) / ChartsInLine + 1) * (OffsetsBetweenCharts.Y + BackgroundChartSize.Y) + OffsetsBetweenCharts.Y);
	UVs.Emplace(0.f, 1.f);

	Background->UpdateMeshSection(0, Vertices, EmptyNormals, UVs, EmptyColors, EmptyTangents);

	UMaterialInstanceDynamic* MID = Background->CreateDynamicMaterialInstance(0, BackgroundMaterial);
	MID->SetVectorParameterValue("Base Color Top", BackgroundTopColor);
	MID->SetVectorParameterValue("Base Color Bottom", BackgroundBottomColor);
}