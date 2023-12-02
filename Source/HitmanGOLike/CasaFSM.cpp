#include "CasaFSM.h"

CasaFSM::CasaFSM()
{
}

CasaFSM::~CasaFSM()
{
}

void CasaFSM::ChangeState(CasaState* Dest)
{
	if (CurrentState)
	{
		CurrentState->FEndDelegate.ExecuteIfBound();
	}
	CurrentState = Dest;
	CurrentState->FStartDelegate.ExecuteIfBound();
}
