#pragma once

#include "GameStateMachine\StateFactory.h"

namespace Anubis
{
	class VSMStateFactory : public StateFactory
	{
	public:

		// =======	Implement State Factory  ======= //
		AVIRTUAL State * VCreateState(AINT32 i32Id) const;
	};
}; //Anubis