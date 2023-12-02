// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CasaState.h"

/**
 * 
 */
class HITMANGOLIKE_API CasaFSM
{
public:
	CasaFSM();

	~CasaFSM();

	class CasaState* CurrentState = nullptr;

	void ChangeState(FString StateName, bool PlayTransition = true);

protected:
	void ChangeStateTo(CasaState* dest, bool PlayTransition = true);
	class CasaState* GetState(const FString StateName);

public:

	class TArray<CasaState*> States;
};
