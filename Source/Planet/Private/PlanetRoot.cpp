// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetRoot.h"
#include "ProceduralMeshComponent.h"
#include "Math/Rotator.h"

// Sets default values
APlanetRoot::APlanetRoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComponent");
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->bUseComplexAsSimpleCollision = true;


	//FaceRotations.SetNum(6);
	FaceRotations.Add(FRotator(0, 0, 90));
	FaceRotations.Add(FRotator(0, 0, 180));
	FaceRotations.Add(FRotator(0, 0, 270));
	FaceRotations.Add(FRotator(0, 0, 360));
	FaceRotations.Add(FRotator(90, 0, 0));
	FaceRotations.Add(FRotator(270, 0, 0));
}

// Called when the game starts or when spawned
void APlanetRoot::BeginPlay()
{
	Super::BeginPlay();

	PlanetLocation = GetActorLocation();
	
	CreateGridMesh(Resolution, Resolution, Triangles, Vertices, UV0, 1);
}




void APlanetRoot::CreateGridMesh(int32 pNumX, int32 pNumY, TArray<int32>& pTriangles, TArray<FVector>& pVertices, TArray<FVector2D>& pUVs, float GridSpacing)
{
	Triangles.Empty();
	Vertices.Empty();
	UV0.Empty();

	int32 MaxSideCount = 6;
	int32 CurrentSide = 0;

	TArray<FVector> BaseVertices;	
	FVector2D Extent = FVector2D((pNumX - 1) * GridSpacing, (pNumY - 1) * GridSpacing) / 2;


	//Verticles
	for (int i = 0; i < pNumY; i++)
	{
		for (int j = 0; j < pNumX; j++)
		{					
			BaseVertices.Add(FVector((float)j * GridSpacing - Extent.X, (float)i * GridSpacing - Extent.Y, 0));
		}
	}


	//Triangles
	for (int i = 0; i < pNumY - 1; i++)
	{
		for (int j = 0; j < pNumX - 1; j++)
		{
			int idx = j + (i * pNumX);
			Triangles.Add(idx);
			Triangles.Add(idx + pNumX);
			Triangles.Add(idx + 1);

			Triangles.Add(idx + 1);
			Triangles.Add(idx + pNumX);
			Triangles.Add(idx + pNumX + 1);
		}
	}
	

	//Create mesh
	float P = (pNumX % 2)? 0.7 : 1.15;

	while (CurrentSide < 6)
	{
		for (int i = 0; i < BaseVertices.Num(); i++)
		{
			FVector ResultVertices = BaseVertices[i] + FVector(0, 0, (pNumX - P)/2);
			ResultVertices = FaceRotations[CurrentSide].RotateVector(ResultVertices);
			ResultVertices.Normalize();
			ResultVertices = ResultVertices * PlanetRadius;

			Vertices.Add(ResultVertices);
			VerticesBuffer.Add(ResultVertices);
		}
		Mesh->CreateMeshSection(CurrentSide, Vertices, Triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), bCreateCollision);
		Vertices.Empty();

		CurrentSide++;
	}
	
	Sections = VerticesBuffer.Num()/6;

}


// Called every frame
void APlanetRoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

//void APlanetRoot::CreateVertices()
//{
//	for (int32 x = 0; x <= XSize; x++)
//	{
//		for (int32 y = 0; y <= YSize; y++)
//		{
//			Vertices.Add(FVector(x * PlanetRadius, y * PlanetRadius, 0));
//		}
//	}
//}
//
//void APlanetRoot::CreateTriangles()
//{
//	int32 Vertex = 0;
//
//	for (int32 x = 0; x < Resolution; x++)
//	{
//		for (int32 y = 0; y < YSize; y++)
//		{
//			Triangles.Add(Vertex);
//			Triangles.Add(Vertex + 1);
//			Triangles.Add(Vertex + YSize + 1);
//			Triangles.Add(Vertex + 1);
//			Triangles.Add(Vertex + YSize + 2);
//			Triangles.Add(Vertex + YSize + 1);
//
//			Vertex++;
//		}
//
//		Vertex++;
//	}
//	
//}









//TriangleSize = 100;
//LodResolution = 128;
//LodResolution = FMath::RoundToInt(((float)LodResolution / 4.f)) * 4;
//WorldScapeLod[i]->Init(i, LodResolution(128), TriangleSize* (2 ^ i)(100), (PPos* PlanetPlanetRadius - PlanetLocation), TerrainMaterial, PlanetPlanetRadius, PlanetLocation, false, false);

//void UWorldScapeLod::Init(int pLod, int TriResolution(128), double Size(100), DVector pRelativePosition, FWSMaterialLodArray pMaterialLodArray, double PlanetPlanetRadius, DVector PlanetLocation, bool pWaterBody, bool bCollision)
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//USTRUCT(BlueprintType)
//struct FWorldScapeMeshTangent
//{
//	GENERATED_BODY()
//public:
//
//	// Direction of X tangent for this vertex 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tangent)
//		FVector TangentX;
//
//	// Bool that indicates whether we should flip the Y tangent when we compute it using cross product 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tangent)
//		bool bFlipTangentY;
//
//	FWorldScapeMeshTangent()
//		: TangentX(1.f, 0.f, 0.f)
//		, bFlipTangentY(false)
//	{}
//
//	FWorldScapeMeshTangent(float X, float Y, float Z)
//		: TangentX(X, Y, Z)
//		, bFlipTangentY(false)
//	{}
//
//	FWorldScapeMeshTangent(FVector InTangentX, bool bInFlipTangentY)
//		: TangentX(InTangentX)
//		, bFlipTangentY(bInFlipTangentY)
//	{}
//};









//TArray<FWorldScapeMeshTangent> Tangents;
//
//FVector PPos = FVector(0, 0, 0);
//PPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
//
//
//FVector SurfaceNormal = RelativePosition.ToFVector();   RelativePos((PPos* PlanetPlanetRadius - PlanetLocation))
//SurfaceNormal.Normalize();
//
//FVector Tangent = FVector::CrossProduct(SurfaceNormal, FVector::RightVector);
//
//if (Tangent.SizeSquared() <= FLT_EPSILON)
//{
//	Tangent = FVector::CrossProduct(SurfaceNormal, FVector::UpVector);
//}
//Tangent.Normalize();
//FVector BiTangent = FVector::CrossProduct(SurfaceNormal, Tangent);
//
//double StepSize = (double)Size / (double)TriResolution; (0,78125)
//double HalfSize = Size * 0.5;  (50)
//int MinY = 0, MinX = 0, MaxY = 0, MaxX = 0;
//int Width = 2;
//
//
//FVector X, Y;
//
//MinY = 4;
//MaxY = 6;
//MinX = 0;
//MaxX = 6; 
//
//int32 IndexOffset = 0;
//
//for (int32 y = MinY; y < MaxY; y++)
//{
//	for (int32 x = MinX; x < MaxX; x++)
//	{
//		IndexOffset++; 
//		X = Tangent * (x * 12,5 - 4);
//		Y = BiTangent * (y * 12, 5 - 4);
//		PositionNormal = DVector(X + Y);
//		PositionNormal = PositionNormal + RelativePosition;
//		PositionNormal.Normalize();
//		Vertices.Add(((PositionNormal * PlanetPlanetRadius) - RelativePosition).ToFVector());
//		Normals.Add(FVector(0.0f, 0.0f, 1.0f));
//		UV.Add(FVector2D(x * uvSpacing, y * uvSpacing));
//		VertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
//		Tangents.Add(FWorldScapeMeshTangent(Tangent.X, Tangent.Y, Tangent.Z));
//	}
//}
//
//
//Width = (MaxX - MinX);
//
//
//for (int32 y = 0; y < (MaxY - MinY) - 1; y++)
//{
//	for (int32 x = 0; x < (MaxX - MinX) - 1; x++)
//	{
//
//		Triangles.Add(PreviousOffset + x + (y * (Width)));					//current vertex
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + Width);			//current vertex + row
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + Width + 1);		//current vertex + row + one right
//
//		Triangles.Add(PreviousOffset + x + (y * (Width)));					//current vertex
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + Width + 1);		//current vertex + row + one right
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + 1);				//current vertex + one right
//
//	}
//}





















//TArray<FWorldScapeMeshTangent> Tangents;



//Patch A (Up to down strip)
//TArray<FVector> VerticesPA;
//TArray<FVector> VerticesNormalPA;
//TArray<FLinearColor>  VerticesColorPA;
//TArray<FVector2D> UVPA;
//TArray<FWorldScapeMeshTangent> TangentsPA;
//
////Patch B (Left to Right Strip)
//TArray<FVector> VerticesPB;
//TArray<FVector> VerticesNormalPB;
//TArray<FLinearColor>  VerticesColorPB;
//TArray<FVector2D> UVPB;
//TArray<FWorldScapeMeshTangent> TangentsPB;



//void APlanetRoot::GenerateMesh(int32 pSectionIndex, const TArray<FVector>& pVertices, const TArray<int32>& pTriangles, const TArray<FVector>& pNormals, 
//	const TArray<FVector2D>& pUV0, const TArray<FColor>& pVertexColor, const TArray<FProcMeshTangent>& pTangents, bool pbCreateCollision)
//{
//	//Mesh->CreateMeshSection(pSectionIndex, pVertices, pTriangles, pNormals, pUV0, pVertexColor, pTangents, pbCreateCollision);
//}




//
//	Struct used to specify a tangent vector for a vertex
//	The Y tangent is computed from the cross product of the vertex normal (Tangent Z) and the TangentX member.
//


//USTRUCT(BlueprintType)
//struct FWorldScapeMeshTangent
//{
//	GENERATED_BODY()
//public:
//
//	// Direction of X tangent for this vertex 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tangent)
//		FVector TangentX;
//
//	// Bool that indicates whether we should flip the Y tangent when we compute it using cross product 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tangent)
//		bool bFlipTangentY;
//
//	FWorldScapeMeshTangent()
//		: TangentX(1.f, 0.f, 0.f)
//		, bFlipTangentY(false)
//	{}
//
//	FWorldScapeMeshTangent(float X, float Y, float Z)
//		: TangentX(X, Y, Z)
//		, bFlipTangentY(false)
//	{}
//
//	FWorldScapeMeshTangent(FVector InTangentX, bool bInFlipTangentY)
//		: TangentX(InTangentX)
//		, bFlipTangentY(bInFlipTangentY)
//	{}
//};




//........................................................Vertex


// One vertex for the procedural mesh, used for storing data internally 
// 
// 
//USTRUCT(BlueprintType)
//struct FWorldScapeMeshVertex
//{
//	GENERATED_BODY()
//public:
//
//	// Vertex position
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FVector Position;
//
//	// Vertex normal
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FVector Normal;
//
//	// Vertex tangent
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FWorldScapeMeshTangent Tangent;
//
//	// Vertex color
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FColor Color;
//
//	// Vertex texture co-ordinate
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FVector2D UV0;
//
//	// Vertex texture co-ordinate
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FVector2D UV1;
//
//	// Vertex texture co-ordinate
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FVector2D UV2;
//
//	// Vertex texture co-ordinate
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vertex)
//		FVector2D UV3;
//
//
//	FWorldScapeMeshVertex()
//		: Position(0.f, 0.f, 0.f)
//		, Normal(0.f, 0.f, 1.f)
//		, Tangent(FVector(1.f, 0.f, 0.f), false)
//		, Color(255, 255, 255)
//		, UV0(0.f, 0.f)
//		, UV1(0.f, 0.f)
//		, UV2(0.f, 0.f)
//		, UV3(0.f, 0.f)
//	{}
//};

//........................................................






//........................................................Section

// One section of the procedural mesh. Each material has its own section. 
//USTRUCT()
//struct FWorldScapeMeshSection
//{
//	GENERATED_BODY()
//public:
//
//	// Vertex buffer for this section 
//	UPROPERTY()
//		TArray<FWorldScapeMeshVertex> PlanetVertexBuffer;
//
//	// Index buffer for this section 
//	UPROPERTY()
//		TArray<uint32> PlanetIndexBuffer;
//	// Local bounding box of section 
//	UPROPERTY()
//		FBox SectionLocalBox;
//
//	// Should we build collision data for triangles in this section 
//	UPROPERTY()
//		bool bEnableCollision;
//
//	// Should we display this section 
//	UPROPERTY()
//		bool bSectionVisible;
//
//	FWorldScapeMeshSection()
//		: SectionLocalBox(ForceInit)
//		, bEnableCollision(false)
//		, bSectionVisible(true)
//	{}
//
//	// Reset this section, clear all mesh info. 
//	void Reset()
//	{
//		PlanetVertexBuffer.Empty();
//		PlanetIndexBuffer.Empty();
//		SectionLocalBox.Init();
//		bEnableCollision = false;
//		bSectionVisible = true;
//	}
//};

//............................................................



//FVector SurfaceNormal = RelativePosition.ToFVector();   RelativePos((PPos* PlanetPlanetRadius - PlanetLocation))
//
//SurfaceNormal.Normalize();
//
//FVector Tangent = FVector::CrossProduct(SurfaceNormal, FVector::RightVector);
//
//if (Tangent.SizeSquared() <= FLT_EPSILON)
//{
//	Tangent = FVector::CrossProduct(SurfaceNormal, FVector::UpVector);
//}
//Tangent.Normalize();
//
//FVector BiTangent = FVector::CrossProduct(SurfaceNormal, Tangent);



//TriangleSize = 100;
//LodResolution = 128;
//LodResolution = FMath::RoundToInt(((float)LodResolution / 4.f)) * 4;
//WorldScapeLod[i]->Init(i, LodResolution(128), TriangleSize* (2 ^ i)(100), (PPos* PlanetPlanetRadius - PlanetLocation), TerrainMaterial, PlanetPlanetRadius, PlanetLocation, false, false);
//void UWorldScapeLod::Init(int pLod, int TriResolution(128), double Size(100), DVector pRelativePosition, FWSMaterialLodArray pMaterialLodArray, double PlanetPlanetRadius, DVector PlanetLocation, bool pWaterBody, bool bCollision)

//double StepSize = (double)Size / (double)TriResolution; (0,78125)
//double HalfSize = Size * 0.5;  (50)
//int MinY = 0, MinX = 0, MaxY = 0, MaxX = 0;
//int Width = 2;

//MinY = (3 * TriResolution(128)) / 4 - 2; (96)
//MaxY = TriResolution - 2; (126)
//MinX = 0;
//MaxX = TriResolution - 2; (126)

//for (int32 y = MinY(96); y(96) < MaxY(126); y++)
//{
//	for (int32 x = MinX(0); x(0) < MaxX(126); x++)
//	{
//	(0) IndexOffset++; (1)
//		X = Tangent * (x * StepSize(0,78125) - HalfSize);
//		Y = BiTangent * (y * StepSize(0,78125) - HalfSize(0,78125));
//		PositionNormal = DVector(X + Y);
//		PositionNormal = PositionNormal + RelativePosition;
//		PositionNormal.Normalize();
//		Vertices.Add(((PositionNormal * PlanetPlanetRadius) - RelativePosition).ToFVector());
//		Normals.Add(FVector(0.0f, 0.0f, 1.0f));
//		UV.Add(FVector2D(x * uvSpacing, y * uvSpacing));
//		VertexColors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
//		Tangents.Add(FWorldScapeMeshTangent(Tangent.X, Tangent.Y, Tangent.Z));
//	}
//}
//
//
//Width = (MaxX - MinX);
//
//
//for (int32 y = 0; y < (MaxY - MinY) - 1; y++)
//{
//	for (int32 x = 0; x < (MaxX - MinX) - 1; x++)
//	{
//
//		Triangles.Add(PreviousOffset + x + (y * (Width)));					//current vertex
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + Width);			//current vertex + row
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + Width + 1);		//current vertex + row + one right
//
//		Triangles.Add(PreviousOffset + x + (y * (Width)));					//current vertex
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + Width + 1);		//current vertex + row + one right
//		Triangles.Add(PreviousOffset + x + (y * (Width)) + 1);				//current vertex + one right
//
//	}
//}








//void APlanetRoot::GenerateMesh()
//{
//	FVector PPos = FVector(0, 0, 0);
//
//	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()) && IsValid(GetWorld()->GetFirstPlayerController()->GetPawn()))
//	{
//		PPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
//	}
//
//	PPos = PPos + PlanetLocation;
//	PPos.Normalize();
//
//	FVector Normal = FVector(0, 0, 1);
//
//
//	if (IsValid(GetWorld()))
//	{
//		MaterialTerrainInstances.Empty();
//		for (int32 i = 0; i < MaxLod; i++)
//		{
//			WorldScapeLod.Add(NewObject <UWorldScapeLod>(this));
//			WorldScapeLod[i]->SetIsReplicated(true);
//			WorldScapeLod[i]->AttachToComponent(TransformKeeper, FAttachmentTransformRules::KeepRelativeTransform);
//			WorldScapeLod[i]->SetMobility(EComponentMobility::Movable);
//			WorldScapeLod[i]->Mesh->SetMobility(EComponentMobility::Movable);
//			WorldScapeLod[i]->Mesh->SetVisibility(!bDisplayCollision);
//			WorldScapeLod[i]->Mesh->AttachToComponent(TransformKeeper, FAttachmentTransformRules::KeepRelativeTransform);
//			WorldScapeLod[i]->SetRelativeLocation(FVector(0, 0, 0));
//			WorldScapeLod[i]->RegisterComponentWithWorld(GetWorld());
//			WorldScapeLod[i]->Init(i, LodResolution, TriangleSize * (2 ^ i), (PPos * PlanetPlanetRadiusCode - PlanetLocation), TerrainMaterial, PlanetPlanetRadiusCode, PlanetLocation, false, false);
//
//			if (WorldScapeLod[i]->Mesh->GetMaterial(0) != nullptr)
//			{
//				UMaterialInstanceDynamic* MID = WorldScapeLod[i]->Mesh->CreateDynamicMaterialInstance(0, WorldScapeLod[i]->Mesh->GetMaterial(0));
//				WorldScapeLod[i]->Mesh->SetMaterial(1, MID);
//				WorldScapeLod[i]->Mesh->SetMaterial(2, MID);
//				MaterialTerrainInstances.Add(MID);
//				MID->SetVectorParameterValue("PlanetLocation", PlanetLocation.ToFVector());
//			}
//
//
//		}
//
//	}
//}

