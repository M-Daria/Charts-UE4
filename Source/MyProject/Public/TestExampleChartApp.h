// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChartViewComponent.h"
#include "GameFramework/Actor.h"
#include "TestExampleChartApp.generated.h"

UCLASS()
class MYPROJECT_API ATestExampleChartApp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestExampleChartApp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UChartViewComponent* ChartView;
};
