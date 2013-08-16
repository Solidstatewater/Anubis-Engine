#include "VSMDemo_pch.h"
#include "VSMStateFactory.h"
#include "States\ShadowTestState.h"

using namespace Anubis;

State * VSMStateFactory::VCreateState(AINT32 i32Id) const
{
	switch (i32Id)
	{
	case GS_VSMTEST:
		return new ShadowTestState();
	};

	return StateFactory::VCreateState(i32Id);
}