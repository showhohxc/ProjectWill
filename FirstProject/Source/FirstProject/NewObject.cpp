// Fill out your copyright notice in the Description page of Project Settings.


#include "NewObject.h"

UNewObject::UNewObject()
{
	MyFloat = 0.0f;
}

void UNewObject::MyFunction()
{
	UE_LOG(LogTemp, Log, TEXT("This is our warning text"));
	//UE_LOG(LogTemp, Warning, TEXT("Message"));
}