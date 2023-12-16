#include "CasaFSM.h"

CasaFSM::CasaFSM()
{
}

CasaFSM::~CasaFSM()
{
}

void CasaFSM::ChangeStateTo(CasaState* Dest, bool PlayTransition)
{
	if (CurrentState && PlayTransition)
	{
		CurrentState->FEndDelegate.ExecuteIfBound();
	}
	CurrentState = Dest;
	if (PlayTransition) {
		CurrentState->FStartDelegate.ExecuteIfBound();
	}
}

void CasaFSM::DebugCurrentState()
{
	//UE_LOG(LogTemp, Warning, TEXT("Current State: %s"), *CurrentState->Name);
}

void CasaFSM::SetNextState(FString Dest)
{
	CasaState* St = GetState(Dest);
	if (St != nullptr)
	{
		NextState = St;
	}
}

void CasaFSM::GoToNextState()
{
	ChangeStateTo(NextState, true);
}

void CasaFSM::ChangeState(FString StateName, bool PlayTransition)
{
	CasaState* St = GetState(StateName);
	if (St != nullptr)
	{
		FString Dummy = "No state";
		UE_LOG(LogTemp, Warning, TEXT("Changing State from : % s To : % s"), CurrentState ? *CurrentState->Name : *Dummy, *StateName);
		ChangeStateTo(St, PlayTransition);
	}
	else {
		
	}
}

CasaState* CasaFSM::GetState(const FString StateName)
{
	for (CasaState* St : States)
	{
		if (St->Name == StateName) {
			return St;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("State %s Not found!"), *StateName);
	return nullptr;
}
