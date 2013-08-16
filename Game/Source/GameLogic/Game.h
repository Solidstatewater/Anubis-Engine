#pragma once
//====================================================================================
//Game.h : Declare game logic type
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

#include "../Entities/EntityManager.h"
#include "../GameStateMachine/StateFactory.h"
#include "../GameStateMachine/StateMachine.h"
#include "Views.h"

#include "../../../Math/Source/Mat4x4.h"

namespace Anubis
{
	class EntityManager;
	class Game
	{
	protected:
		/** -------------------
			Entities
		----------------------- **/
		EntityId		m_lastEntityId; //id of last added actor
		EntityManager*	m_pEntityManager; //entity manager

		/** -------------------
			States
		----------------------- **/
		StateFactory*	m_pStateFactory;
		StateMachine*	m_pStateMachine;

		//keep data about the next game state
		struct NextGameState
		{
			AINT32		m_i32Id;	//next state id
			AUINTPTR	m_uptrData; //next state data
			ABOOL		m_bSetState;//true if state change is necessary
		};
		NextGameState	m_ngsState;

		/** -------------------
			Views
		----------------------- **/
		Views		m_views;

	public:
		//constructor and destructor
		Game();
		AVIRTUAL ~Game();

		//============================
		// Initializing
		//============================

		//main initialization
		AVIRTUAL ABOOL VInitialize();

		//game states
		AVOID SetStateFactory(StateFactory * pStateFactory);
		AVOID SetNextState(AINT32 i32StateId, AUINTPTR uptrData = 0);
		
		/*==
		//App stuff
		==*/
		AVIRTUAL LRESULT	VMsgProc(SystemMessage * msg);

		/*==
		//Updating methods
		==*/
		AVIRTUAL AVOID		VUpdate(const AREAL64 r64Time, const AREAL64 r64ElapsedTime);

		/*==
		//Rendering methods
		==*/
		AVIRTUAL AVOID		VRender(Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime);
		AVIRTUAL AVOID		VRenderDebug();

///////////////////////////////////////////////////////////////////////////////////////////

		/*==
		//Entities management
		==*/
		AVIRTUAL EntityPtr	VAddEntity(EntityPtr pEntity);
		AVIRTUAL EntityPtr	VGetEntity(const EntityId id);
		AVIRTUAL ABOOL		VRemoveEntity(const EntityId id);

		//Update entity transform matrix
		AVIRTUAL AVOID		VUpdateEntity(const EntityId id, Mat4x4 const & transform, AREAL64 r64CurrentTime);
		AINLINE  EntityId	GetNewEntityId() { return ++m_lastEntityId; }

		/*==
		//States management
		==*/
		ABOOL SetState(AINT32 i32StateId);

		/*==
		//Views management
		==*/
		AVIRTUAL ViewPtr	VAddGameView(shared_ptr<IView> pView);
		AVIRTUAL AVOID		VRemoveGameView(shared_ptr<IView> pView);
		const	 Views *	 GetViews() const { return &m_views; }
	};
}; //Anubis