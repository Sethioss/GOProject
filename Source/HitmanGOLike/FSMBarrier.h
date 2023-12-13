// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


/**
 * 
 */
class HITMANGOLIKE_API FSMBarrier
{
public:
	static TArray<AActor*> BarrieredObjects;

	FSMBarrier();
	~FSMBarrier();
};
