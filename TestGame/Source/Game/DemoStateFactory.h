#pragma once

#include "GameStateMachine\StateFactory.h"

namespace Anubis
{
	class DemoStateFactory : public StateFactory
	{
	public:

		// =======	Implement State Factory  ======= //
		AVIRTUAL State * VCreateState(AINT32 i32Id) const;
	};
}; //Anubis