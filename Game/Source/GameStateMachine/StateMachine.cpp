//====================================================================================
//StateMachine.cpp 
//
//This code is part of Anubis Engine.
//
//Anubis Engine is a free game engine created as a fan project to be
//awesome platform for developing games!
//
//All sources can be found here:
//	https://github.com/Dgek/Engine
//
//Demos based on Anubis Engine can be found here:
//	https://github.com/Dgek/Demos
//
//Copyright (c) 2013, Muralev Evgeny
//All rights reserved.
//
//Redistribution and use in source and binary forms, with
//or without modification, are permitted provided that the
//following conditions are met:
//
//Redistributions of source code must retain the above copyright notice,
//this list of conditions and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice,
//this list of conditions and the following disclaimer in the documentation
//and/or other materials provided with the distribution.
//
//Neither the name of the Minotower Games nor the names of its contributors 
//may be used to endorse or promote products derived from this software 
//without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY MURALEV EVGENY "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
//THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED. IN NO EVENT SHALL MURALEV EVGENY BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//====================================================================================

#include "Game_pch.h"
#include "StateMachine.h"

using namespace Anubis;

//constructor
StateMachine::StateMachine()
{
	m_pCurrentState = NULL;
}

AVOID StateMachine::Release(Game * pGame)
{
	ReleaseCurrentState(pGame);
}

//Set a new state
ABOOL StateMachine::SetState(Game * pGame, State * pState, AINT32 i32StateId)
{
	//some error occured deleting current state
	if (!ReleaseCurrentState(pGame))
		return false;

	//assign new state as current
	m_pCurrentState = pState;

	//nothing else is left to do for null states
	if (!pState)
		return true;

	//assign state id
	m_pCurrentState->m_i32Id = i32StateId;

	//initialize new game state
	m_pCurrentState->VInitialize(pGame);

	return true;
}

//Destroy current state
ABOOL StateMachine::ReleaseCurrentState(Game * pGame)
{
	//current state is null
	if (!m_pCurrentState)
		return true;

	//destroy current state and release memory
	m_pCurrentState->VRelease(pGame);
	delete m_pCurrentState;

	//set current state to NULL
	m_pCurrentState = NULL;

	return true;
}

//Update
AVOID StateMachine::VUpdate(Game * pGame, AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
	if (m_pCurrentState)
		m_pCurrentState->VUpdate(pGame, r64Time, r64ElapsedTime);
}

//Render
AVOID StateMachine::VRender(Game * pGame, Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
	if (m_pCurrentState)
		m_pCurrentState->VRender(pGame, pRenderer, r64Time, r64ElapsedTime);
}