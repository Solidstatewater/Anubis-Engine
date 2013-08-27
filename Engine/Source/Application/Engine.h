#pragma once
//====================================================================================
//Engine.h : Define game engine class
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

#include "../Time/Timer.h"

#include "../../../Renderer/Source/Deferred/DeferredRenderer.h"
#include "../../../Messenger/Source/Messenger.h"
#include "../../../Renderer/Source/Renderer.h"
#include "../../../Physics/Source/PhysicsInterface.h"
#include "../../../Game/Source/GameLogic/Game.h"

#include "../../../Audio/Source/Audio.h"
#include "../../../ResourceManager/Source/ResourceCache.h"

namespace Anubis
{
	class Engine
	{
	public:

		//initialization
		Engine() : m_r64LastUpdate(0), m_r64TimeStep(0.0333), m_bQuitting(false) 
		{
			m_pTimer = new Timer(); //initialize game timer
			m_windowClassName = L"window";
		}

		ABOOL AlreadyRunning(LPCTSTR title);

		//accessors
		HINSTANCE	GetInstance()	const { return m_hInstance; }

		//Win32 stuff
		ASTATIC LRESULT ACALLBACK WndProc(HWND hwnd, AUINT32 msg, WPARAM wParam, LPARAM lParam);

		ABOOL MsgProc(SystemMessage * message);

		AVOID Update(AREAL64 r64Time, AREAL64 r64ElapsedTime);
		AVOID Render(AREAL64 r64Time, AREAL64 r64ElapsedTime);

		/*============================================
		//Engine Initialization
		=============================================*/
		AVOID InitializeWindowAndGame(HINSTANCE hInstance, AINT32 showWnd, AINT32 width, AINT32 height, ABOOL isWindowed, Game* pGame);
		AVOID InitializeComponents(IAudio* pAudio, Renderer* pRenderer, IPhysics* pPhysics, Messenger* pMessenger, ResourceCache* pCache);

		AVOID InitializeWindow(HINSTANCE hInstance, AINT32 showWnd, AINT32 width, AINT32 height, ABOOL isWindowed);
		AVOID InitializeGame(Game * pGame);

		/*============================================
		//Application main loop
		=============================================*/
		AINT32 Run();

		/*============================================
		//Application Shut Down
		=============================================*/
		AVOID Close();

		/*============================================
		//Access Data
		=============================================*/
		AREAL64 GetTimeStep()		const		{ return m_r64TimeStep; }
		AREAL64 GameTimeInSeconds() const		{ return m_pTimer->GetGameTimeSeconds(); }
//		AREAL64 CurrentTimeInSeconds() const	{ return m_pTimer->GetCurrentTimeSeconds(); }

		IAudio*			GetAudio() const			{ return m_pAudio; }
		Messenger*		GetMessenger() const		{ return m_pMessenger; }
		IPhysics*		GetPhysics() const			{ return m_pPhysics; }
		Renderer*		GetRenderer() const			{ return m_pRenderer; }
		ResourceCache*	GetCache()	const			{ return m_pCache; }

		/*============================================
		//Game Initialization
		=============================================*/
		//AVIRTUAL Game* VCreateGame() = 0;
		//AVIRTUAL AVOID RegisterGameSpecificEvents() = 0;

	private:
		//engine own message queue - for cross-platform support
		AQUEUE<SystemMessage*>	m_messageQueue; 

	protected:
		/* Application data */
		
		HWND		m_hwnd;
		LPCTSTR		m_windowClassName;
		HINSTANCE	m_hInstance;
		ABOOL		m_bWindowed;

		AINT32		m_width;
		AINT32		m_height;

		/* Shutdown */
		ABOOL		m_bQuitting;

		/* Timing */
		Timer*		m_pTimer;
		AREAL64		m_r64LastUpdate;
		AREAL64		m_r64TimeStep;

	public:
		/* Engine subsystems */

		////////////////////////////
		//Audio
		IAudio		*m_pAudio;

		////////////////////////////
		//Renderer
		Renderer	*m_pRenderer;

		////////////////////////////
		//Game Logic
		Game		*m_pGame;

		////////////////////////////
		// Event manager
		Messenger	*m_pMessenger;

		////////////////////////////
		//Game Physics
		IPhysics*	m_pPhysics;

		////////////////////////////
		//Resource Cache
		ResourceCache* m_pCache;
	
	};

	extern Engine *g_pEngine;

}; //namespace Anubis