#include "ZMotionBlur_pch.h"
#include "MotionBlurStateFactory.h"
#include "States\MotionBlurState.h"

using namespace Anubis;

State * MotionBlurStateFactory::VCreateState(AINT32 i32Id) const
{
	switch (i32Id)
	{
	case GS_VSMTEST:
		return new MotionBlurState();
	};

	return StateFactory::VCreateState(i32Id);
}