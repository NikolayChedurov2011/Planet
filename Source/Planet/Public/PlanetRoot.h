// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

#include "PlanetRoot.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()

class PLANET_API APlanetRoot : public AActor
{
	GENERATED_BODY()
	
public:	

	APlanetRoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
		double PlanetRadius = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = 8))
		float Resolution = 8;
	bool bCreateCollision = true;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UProceduralMeshComponent* Mesh;
	FVector PlanetLocation;
	TArray<FVector> Vertices;
	TArray<FVector> VerticesBuffer;
	int32 Sections;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	UMaterialInterface* Material;
	TArray<FRotator> FaceRotations;
	float AxisPos;
	void CreateGridMesh(int32 pNumX, int32 pNumY, TArray<int32>& pTriangles, TArray<FVector>& pVertices, TArray<FVector2D>& pUVs, float GridSpacing);
};