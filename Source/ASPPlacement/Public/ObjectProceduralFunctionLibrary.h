// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Wall.h"
#include "Room.h"
#include "Worldobject.h"
#include "ObjectProceduralFunctionLibrary.generated.h"

/**
 * 
 */

#define MAXOBJECTS 1000
#define MAXOBJECTINSTANCES 1000

UENUM(BlueprintType)
enum class FEWalls : uint8 {
	WallA UMETA(DisplayName="WallA"),
	WallB UMETA(DisplayName="WallB"),
	WallC UMETA(DisplayName="WallC"),
	WallD UMETA(DisplayName="WallD"),
	None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class FEFaces : uint8 {
	WallA UMETA(DisplayName = "WallA"),
	WallB UMETA(DisplayName = "WallB"),
	WallC UMETA(DisplayName = "WallC"),
	WallD UMETA(DisplayName = "WallD"),
	None UMETA(DisplayName = "None")
};


USTRUCT(BlueprintType)
struct FSObjectInstanceDescription {

	GENERATED_USTRUCT_BODY()

	FSObjectInstanceDescription() : wall(FEWalls::WallA), rotation(FEFaces::None), objectFaceToWall(FEFaces::None) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	FEWalls wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	int32 distanceFromLeftCorner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	FEFaces rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	FEFaces objectFaceToWall;


};


USTRUCT(BlueprintType)
struct FSObjectDescription {

	GENERATED_USTRUCT_BODY()

	FSObjectDescription() : id(0), CtrFaceToWall(FEFaces::None), scale(1) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	int32 id;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LibObjectDescription")
	TArray<FSObjectInstanceDescription> liobjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	UStaticMesh *StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	FVector dimensions;
		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	FEFaces CtrFaceToWall; //Constraint: object face to wall

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibObject Description")
	float scale;
	

};

USTRUCT(BlueprintType)
struct FSWallConstraint {

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibWall Constraint")
	FEWalls wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LibWall Constraint")
	FVector placementconstraint;

};


UCLASS()
class ASPPLACEMENT_API UObjectProceduralFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:		

		UFUNCTION(BlueprintCallable, Category = "Analytics")
		static bool StartSession(const int32 value);
		
		UFUNCTION(BlueprintCallable, Category = "Placement")
	    static  bool Placement(float unit, float WWidth, float RSx,float RSy,TArray<FSObjectDescription> lobjects, TArray<FSWallConstraint> lwctrs, TArray<FSObjectDescription> &newlobjects );

private:

	static void fvectortolint(float unit, FVector vector, std::list<int>& l);
	static Wall::Name UObjectProceduralFunctionLibrary::walltolibwall(FEWalls wall);
	static Wall::Name UObjectProceduralFunctionLibrary::facetolibface(FEFaces face);
	
};
