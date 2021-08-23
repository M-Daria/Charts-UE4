// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestQuadTree.generated.h"

UCLASS()
class MYPROJECT_API ATestQuadTree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestQuadTree();

	UPROPERTY(EditAnywhere)
		FVector2D x;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e) override;
};
