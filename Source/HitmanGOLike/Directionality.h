// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGeneralDirectionEnum : uint8 {
	UP = 0 UMETA(DisplayName = "UP"),
	RIGHT = 1 UMETA(DisplayName = "RIGHT"),
	DOWN = 2 UMETA(DisplayName = "DOWN"),
	LEFT = 3 UMETA(DisplayName = "LEFT"),
	FORWARDS = 4 UMETA(DisplayName = "FORWARDS"),
	LOCALRIGHT = 5 UMETA(DisplayName = "LOCALRIGHT"),
	BACKWARDS = 6 UMETA(DisplayName = "BACKWARDS"),
	LOCALLEFT = 7 UMETA(DisplayName = "LOCALLEFT"),
	VALNUM = 8,
};

class HITMANGOLIKE_API Directionality
{
public:
	Directionality();
	~Directionality();

	static bool IsSame(const enum class EGeneralDirectionEnum& og, const enum class EGeneralDirectionEnum& dest)
	{
		return static_cast<int>(og) == static_cast<int>(dest);
	}
};
