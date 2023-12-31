// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
DECLARE_DELEGATE(FStart);
DECLARE_DELEGATE(FTick);
DECLARE_DELEGATE(FEnd);

class HITMANGOLIKE_API CasaState
{
public:
	CasaState();
	~CasaState();

	FString Name;
	FStart FStartDelegate;
	FTick FTickDelegate;
	FEnd FEndDelegate;

	template <typename T>
	inline void SetStartDelegate(T* Object, TMemFunPtrType<false, T, void()>::Type InFunc) { FStartDelegate.BindUObject(Object, InFunc); }
	template <typename T>
	inline void SetUpdateDelegate(T* Object, TMemFunPtrType<false, T, void()>::Type InFunc) { FTickDelegate.BindUObject(Object, InFunc); }
	template <typename T>
	inline void SetEndDelegate(T* Object, TMemFunPtrType<false, T, void()>::Type InFunc) { FEndDelegate.BindUObject(Object, InFunc); }
};


