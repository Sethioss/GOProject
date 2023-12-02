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

	void ChangeState(CasaState* dest);

public:

	class TArray<CasaState*> States;
};
