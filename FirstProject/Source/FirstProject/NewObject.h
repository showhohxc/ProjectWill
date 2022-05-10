// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NewObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FIRSTPROJECT_API UNewObject : public UObject
{
	GENERATED_BODY()
	
public:
	UNewObject();

	UPROPERTY(BlueprintReadWrite, Category = "Myvariables")
	float MyFloat;

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void MyFunction();
};
