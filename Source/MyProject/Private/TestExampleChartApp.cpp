// Fill out your copyright notice in the Description page of Project Settings.


#include "TestExampleChartApp.h"

ATestExampleChartApp::ATestExampleChartApp()
{
	PrimaryActorTick.bCanEverTick = false;

	ChartView = CreateDefaultSubobject<UChartViewComponent>(TEXT("ChartView"));
	ChartView->SetupAttachment(RootComponent);

}

void ATestExampleChartApp::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<FVector2D> Line1, Line2;

	Line1.Emplace(-50.0, 50.0);		Line2.Emplace(-100.0, 0.0);
	Line1.Emplace(-40.0, 2.0);		Line2.Emplace(-85.0, 15.0);
	Line1.Emplace(-30.0, 25.0);		Line2.Emplace(-80.0, 0.0);
	Line1.Emplace(-20.0, 10.0);		Line2.Emplace(-70.0, 0.0);
	Line1.Emplace(-10.0, 15.0);		Line2.Emplace(-65.0, 20.0);
	Line1.Emplace(0.0, 0.0);		Line2.Emplace(-60.0, -10.0);
	Line1.Emplace(20.0, 20.0);		Line2.Emplace(-30.0, 20.0);
	Line1.Emplace(30.0, 55.0);		Line2.Emplace(2.0, 40.0);
	Line1.Emplace(50.0, 90.0);		Line2.Emplace(80.0, 80.0);
	Line1.Emplace(80.0, 54.0);		Line2.Emplace(90.0, 33.0);
	Line1.Emplace(90.0, 45.0);		Line2.Emplace(100.0, 24.0);
	Line1.Emplace(100.0, 24.0);		Line2.Emplace(120.0, -2.0);
	Line1.Emplace(110.0, 40.0);		Line2.Emplace(140.0, 0.0);
	Line1.Emplace(120.0, 25.0);		Line2.Emplace(150.0, 32.0);
	Line1.Emplace(130.0, -7.0);		Line2.Emplace(160.0, 0.0);
	Line1.Emplace(140.0, -13.0);	Line2.Emplace(170.0, 22.0);
	Line1.Emplace(150.0, 27.0);
	Line1.Emplace(160.0, 25.0);
	Line1.Emplace(170.0, 69.0);
	Line1.Emplace(180.0, 30.0);
	Line1.Emplace(190.0, 22.0);


	ChartView->CreateChartViewport();

	ChartView->AddLine(0, Line1);
	ChartView->AddLine(1, Line2);
	ChartView->SetXRange(FVector2D(0.0, 1000.0));
	ChartView->SetYRange(FVector2D(0.0, 2.0));
	ChartView->SetXAutoscale(true);
	ChartView->SetYAutoscale(true);
	ChartView->SetViewportSize(FVector2D(600.0, 400.0));

	ChartView->SetChartBackgroundColor(FLinearColor::Black);
	ChartView->SetChartGridColor(FLinearColor(0.05, 0.05, 0.05));
	ChartView->SetChartGridType(EChartGridType::Solid);
	ChartView->SetTextColor(EChartTextNames::AxisValues, FColor::Black);
	ChartView->SetTextColor(EChartTextNames::Title, FColor::Black);
	ChartView->SetTextColor(EChartTextNames::XLabel, FColor::Black);
	ChartView->SetTextColor(EChartTextNames::YLabel, FColor::Black);
	ChartView->SetTextSize(EChartTextNames::Title, 40.0);
	ChartView->SetTextTitle(FText::FromString("Random Values"));
	ChartView->SetTextXLabel(FText::FromString("X-Axis"));
	ChartView->SetTextYLabel(FText::FromString("Y-Axis"));

	ChartView->SetLineColor(0, FLinearColor::MakeFromHSV8(26, 255, 255));
	ChartView->SetLineColor(1, FLinearColor::MakeFromHSV8(253, 255, 255));
	ChartView->SetLineType(0, EChartLineType::Solid);
	ChartView->SetLineType(1, EChartLineType::Dashed);
	ChartView->SetLineDashInterval(1, 5.0);
	ChartView->SetLineDashLength(1, 8.0);
	ChartView->SetLineWidth(0, 2.0);
	ChartView->SetLineWidth(1, 2.0);
}