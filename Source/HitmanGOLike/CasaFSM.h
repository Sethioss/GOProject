// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CasaState.h"
#include "FSMBarrier.h"

/**
 * 
 */
class HITMANGOLIKE_API CasaFSM
{
public:
	CasaFSM();

	~CasaFSM();

	class CasaState* CurrentState = nullptr;
	class CasaState* NextState = nullptr;

	void ChangeState(FString StateName, bool PlayTransition = true);
	void SetNextState(FString Dest);
	void GoToNextState();

protected:
	void ChangeStateTo(CasaState* Dest, bool PlayTransition = true);
	class CasaState* GetState(const FString StateName);

public:

	class TArray<CasaState*> States;


};
