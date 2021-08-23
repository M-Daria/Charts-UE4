// Fill out your copyright notice in the Description page of Project Settings.


#include "ChartStorageComponent.h"

FLineDataLoader::FLineDataLoader(const TArray<FVector2D>& InVertices)
{

}


UChartStorageComponent::UChartStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UChartStorageComponent::AddLine(int32 Index, const TArray<FVector2D>& InVertices)
{

}

void UChartStorageComponent::UpdateLine(int32 Index, const TArray<FVector2D>& InVertices)
{

}

void UChartStorageComponent::ClearLine(int32 Index, const TArray<FVector2D>& InVertices)
{

}

void UChartStorageComponent::ClearAllLines()
{

}