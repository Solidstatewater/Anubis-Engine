#include "TestGame_pch.h"
#include "DemoStateFactory.h"
#include "States\GameTestState.h"

using namespace Anubis;

State * DemoStateFactory::VCreateState(AINT32 i32Id) const
{
	switch (i32Id)
	{
	case GS_GAMETEST:
		return new GameTestState();
	};

	return StateFactory::VCreateState(i32Id);
}