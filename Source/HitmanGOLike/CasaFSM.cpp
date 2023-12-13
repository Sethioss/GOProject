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
		ChangeStateTo(St, PlayTransition);
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
