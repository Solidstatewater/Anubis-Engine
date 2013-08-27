//====================================================================================
//Game.cpp : Implement game logic
//
//This code is part of Anubis Engine.
//
//Anubis Engine is a free game engine created as a fan project to be
//awesome platform for developing games!
//
//All sources can be found here:
//	https://github.com/Dgek/Anubis-Engine
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
#include "Game.h"

using namespace Anubis;

Game::Game() : m_lastEntityId(0)
{
	m_pStateMachine = NULL;
	m_pEntityManager = NULL;
}

Game::~Game()
{
	m_pStateMachine->Release(this);
	m_pStateMachine = NULL;
}

ABOOL Game::VInitialize()
{
	//create state machine
	m_pStateMachine = new StateMachine();

	//create entity manager
	m_pEntityManager = new EntityManager();

	return true;
}

LRESULT	Game::VMsgProc(SystemMessage * msg)
{
	for (Views::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
	{
		if ((*iter)->VMsgProc(msg)) 
		{
			return 1;
		}
	}

	return 0;
}

AVOID Game::VUpdate(const AREAL64 r64Time, const AREAL64 r64ElapsedTime)
{
	//change game state if needed
	if (m_ngsState.m_bSetState)
	{
		m_ngsState.m_bSetState = false;
		SetState(m_ngsState.m_i32Id);
	}

	//update a game state
	m_pStateMachine->VUpdate(this, r64Time, r64ElapsedTime);

	const AUINT32 deltaMilliseconds = AUINT32(r64ElapsedTime * 1000.0f);
	for (Views::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
		(*iter)->VUpdate(deltaMilliseconds);

	//update entities
	m_pEntityManager->VUpdate(r64Time, r64ElapsedTime);
}

/********************************
** Entities management
*********************************/
EntityPtr Game::VAddEntity(EntityPtr pEntity)
{
	m_pEntityManager->VAddEntity(pEntity);
	return pEntity;
}

EntityPtr Game::VGetEntity(const EntityId id)
{
	return m_pEntityManager->VGetEntity(id);
}

ABOOL Game::VRemoveEntity(const EntityId id)
{
	return m_pEntityManager->VRemoveEntity(id);
}

AVOID Game::VUpdateEntity(const EntityId id, Mat4x4 const & transform, AREAL64 r64CurrentTime)
{
	m_pEntityManager->VUpdateEntity(id, transform, r64CurrentTime);
}

/********************************
** States management
*********************************/
AVOID Game::SetStateFactory(StateFactory * pStateFactory)
{
	m_pStateFactory = pStateFactory; 
}

AVOID Game::SetNextState(AINT32 i32StateId, AUINTPTR uptrData)
{
	m_ngsState.m_i32Id = i32StateId;
	m_ngsState.m_uptrData = uptrData;
	m_ngsState.m_bSetState = true;
}

ABOOL Game::SetState(AINT32 i32StateId)
{
	//create state
	State * pState = m_pStateFactory->VCreateState(i32StateId);

	//bind state
	return m_pStateMachine->SetState(this, pState, i32StateId);
}

/********************************
** Views management
*********************************/
ViewPtr Game::VAddGameView(ViewPtr pView)
{
	m_views.push_back(pView);
	return pView;
}

AVOID Game::VRemoveGameView(ViewPtr pView)
{
}

/**********************************
** Rendering methods
**********************************/
AVOID Game::VRenderDebug()
{
}

AVOID Game::VRender(Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
	//render current game state
	for (Views::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
		(*iter)->VRender(pRenderer, r64Time, r64ElapsedTime);

	//render a game state
	m_pStateMachine->VRender(this, pRenderer, r64Time, r64ElapsedTime);
}