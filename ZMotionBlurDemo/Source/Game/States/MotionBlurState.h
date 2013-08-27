#pragma once

#include "GameStateMachine\State.h"
#include "Scene.h"

namespace Anubis
{
	class MotionBlurState : public State
	{
	protected:

	public:

		//constructor
		MotionBlurState() : State() {}

		AVIRTUAL AVOID VInitialize( Game * pGame, AUINTPTR uptrData = 0 );										//Initialize game state

		AVIRTUAL AVOID VRelease( Game * pGame );																//Destroy the state

		AVIRTUAL AVOID VUpdate( Game * pGame, AREAL64 r64Time, AREAL64 r64ElapsedTime );						//Update the state

		AVIRTUAL AVOID VRender( Game * pGame, Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime );	//Render State
	};

}; //Anubis