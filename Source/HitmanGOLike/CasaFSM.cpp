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

void CasaFSM::ChangeState(FString StateName, bool PlayTransition)
{
	CasaState* AwaitingPlayerInputState = GetState(StateName);
	if (AwaitingPlayerInputState != nullptr)
	{
		ChangeStateTo(AwaitingPlayerInputState, PlayTransition);
	}
}

CasaState* CasaFSM::GetState(const FString StateName)
{
	for (CasaState* St : this->States)
	{
		if (St->Name == StateName) {
			return St;
		}
	}
	return nullptr;
}
