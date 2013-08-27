//====================================================================================
//Engine.cpp : Implements game engine functionality
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

#include "Engine_pch.h"
#include "../../../Graphics/Source/Final/General.h"
#include "Engine.h"

using namespace Anubis;

AVOID Engine::InitializeWindow(HINSTANCE hInstance, AINT32 showWnd, AINT32 width, AINT32 height, ABOOL isWindowed)
{
	//assign variables
	m_hInstance = hInstance;
	m_bWindowed = isWindowed;
	m_width = width;
	m_height = height;

	//define struct with window properties
	typedef struct _WNDCLASS {
		AUINT32 cbSize;
		AUINT32 style;
		WNDPROC lpfnWndProc;
		AINT32 cbClsExtra;
		AINT32 cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
	} WNDCLASS;

	//fill the WNDCLASS structure
	WNDCLASSEX wc;
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Engine::WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); 

	//register window
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering window class",	
			L"Error", MB_OK | MB_ICONERROR);
	}

	HRESULT hr = HRESULT_FROM_WIN32(GetLastError());

	//create window
	m_hwnd = CreateWindowEx(
		NULL,
		m_windowClassName,
		L"TestGame",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_width, m_height,
		NULL,
		NULL,
		m_hInstance,
		NULL
		); 

	//check wether window was created correctly
	if (!m_hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
	}

	//show and update window
	ShowWindow(m_hwnd, showWnd);
	UpdateWindow(m_hwnd);
}

AVOID Engine::InitializeGame(Game * pGame)
{
	//assign pointer to a game class
	m_pGame = pGame;

	//try to initialize it
	if (!pGame->VInitialize())
	{
		assert(0 && "Error initializing the game!");
	}
}

AVOID Engine::InitializeWindowAndGame(HINSTANCE hInstance, AINT32 showWnd, AINT32 width, AINT32 height, ABOOL isWindowed, Game* pGame)
{
	//Initialize window first
	InitializeWindow(hInstance, showWnd, width, height, isWindowed);

	//Initialize game
	InitializeGame(pGame);
}

AVOID Engine::InitializeComponents(IAudio* pAudio, Renderer* pRenderer, IPhysics* pPhysics, Messenger* pMessenger, ResourceCache* pCache)
{
	//firstly, initialize our resource cache
	if (pCache)
	{
		m_pCache = pCache;
		//m_pCache->Init();
	}

	//initialize audio system
	if (pAudio)
	{
		m_pAudio = pAudio;
		m_pAudio->VInitialize(m_hwnd);
	}

	//initialize messaging system
	m_pMessenger = pMessenger;
	
	//initialize renderer
	m_pRenderer = pRenderer;
	m_pRenderer->VInitialize(m_hwnd, m_width, m_height);

	//Add cameras to the renderer

	const Views * pViews = m_pGame->GetViews();
	for (auto it = pViews->begin(); it != pViews->end(); it++)
	{
		if ((*it)->VHasCamera())
		{
			m_pRenderer->AddCamera((*it)->VGetCamera());
		}
	} 

	//initialize physics
	m_pPhysics = pPhysics;
	m_pPhysics->VInit();
}

LRESULT CALLBACK Engine::WndProc(HWND hwnd, AUINT32 msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE ){
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

ABOOL Engine::MsgProc(SystemMessage * message)
{
	switch (message->m_type)
	{
		case SMT_KeyDown:
		case SMT_KeyUp:
		case SMT_LMouseDown:
		case SMT_LMouseUp:
		case SMT_RMouseDown:
		case SMT_RMouseUp:
		case SMT_MouseMove:
		{
			return m_pGame->VMsgProc(message);
		}
		break;
	};
}

AVOID Engine::Update(AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
	//If we are in quitting state
	if (m_bQuitting)
	{
		//post WM_CLOSE message to the application
		PostMessage(m_hwnd, WM_CLOSE, 0, 0);
		return;
	}

	if (m_pPhysics)
		m_pPhysics->VUpdate(r64ElapsedTime);

	if (m_pGame)
		m_pGame->VUpdate(r64Time, r64ElapsedTime);
}

AVOID Engine::Render(AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
	m_pGame->VRender(m_pRenderer, r64Time, r64ElapsedTime);
	m_pRenderer->VRender();

	//show back buffer
	PresentBackBuffer();
}

AINT32 Engine::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/*********************************
			Application received quit message!
			*********************************/

			//Generate system message
			switch (msg.message)
			{                                                                                                                                                                                                                                                                 
			case WM_QUIT:
				//m_messageQueue.push(QuitMessage());
				Close();
				return msg.wParam;
			case WM_KEYDOWN:
				m_messageQueue.push(new KeyDownMessage(msg.wParam));
				break;
			case WM_KEYUP:
				m_messageQueue.push(new KeyUpMessage(msg.wParam));
				break;
			case WM_RBUTTONDOWN:
				m_messageQueue.push(new RMouseDownMessage(LOWORD(msg.lParam), HIWORD(msg.lParam)));
				break;
			case WM_RBUTTONUP:
				m_messageQueue.push(new RMouseUpMessage(LOWORD(msg.lParam), HIWORD(msg.lParam)));
				break;
			case WM_LBUTTONDOWN:
				m_messageQueue.push(new LMouseDownMessage(LOWORD(msg.lParam), HIWORD(msg.lParam)));
				break;
			case WM_LBUTTONUP:
				m_messageQueue.push(new LMouseUpMessage(LOWORD(msg.lParam), HIWORD(msg.lParam)));
				break;
			case WM_MOUSEMOVE:
				m_messageQueue.push(new MouseMoveMessage(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam)));
				break;
			};

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			while (m_messageQueue.size())
			{
				SystemMessage *pMsg = m_messageQueue.front();
				MsgProc(pMsg);

				m_messageQueue.pop();
				SAFE_DELETE(pMsg);
			};
		}
		else
		{
			/*
				Update Timer
			*/
			m_pTimer->Tick();

			/* ==
				Update with fixed framerate 
										  == */
			AREAL64 r64GameTime = m_pTimer->GetGameTimeSeconds();
			//if ( (r64GameTime - m_r64LastUpdate) > 1.0f) m_r64LastUpdate = r64GameTime;
			//while (r64GameTime - m_r64LastUpdate > m_r64TimeStep)
			if ( (r64GameTime - m_r64LastUpdate) > m_r64TimeStep)
			{
				Update(m_r64LastUpdate, m_r64TimeStep);
				//m_r64LastUpdate += m_r64TimeStep;
				m_r64LastUpdate = r64GameTime;

				m_pPhysics->VSyncData(m_r64LastUpdate);
			}

			/* ==
				Render
					== */
			Render(r64GameTime, m_pTimer->GetDeltaTime());
		}
	}

	return msg.wParam;
}

AVOID Engine::Close()
{
	//SAFE_DELETE(m_pGame);
	//CleanUpGraphics();
	SAFE_DELETE(m_pMessenger);
	DestroyWindow(m_hwnd);
	SAFE_DELETE(m_pTimer);
}