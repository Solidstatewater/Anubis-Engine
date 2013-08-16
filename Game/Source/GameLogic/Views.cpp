//====================================================================================
//Views.cpp
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
#include "Views.h"

#include "../../../Graphics/Source/Final/General.h"

using namespace Anubis;

PlayerView::PlayerView() : m_bHasCamera(false)
{
	m_pMouseHandler = NULL;
	m_pKeyboardHandler = NULL;
	m_pController = NULL;
}

ABOOL PlayerView::VInit()
{
	/*** Create Renderer
	//
	// Implementation at Renderer project
	//
	***/
	//m_pDeferredRenderer = new DeferredRenderer();

	bgColor[0] = 1.0f;
	bgColor[1] = 1.0f;
	bgColor[2] = 1.0f;
	bgColor[3] = 1.0f;

	return true;
}

ABOOL PlayerView::VRestore()
{ 
	return true;
}

AVOID PlayerView::VUpdate(AUINT32 const deltaMilliseconds)
{
	//Update widgets
	for (Widgets::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
	{
		(*it)->VUpdate(deltaMilliseconds);
	}

	if (!m_pController)	return;

	//update controller
	m_pController->VUpdate(deltaMilliseconds);

	//Update only if we have camera
	if (m_bHasCamera)
	{
		//Update camera's position
		Vec vel = m_pController->VGetTargetVelocity();
		m_pCamera->SetPosition(m_pCamera->GetPosition() + vel);

		//Update camera's orientation
		AREAL yaw = m_pController->VGetTargetYaw();
		m_pCamera->SetYaw(yaw);

		AREAL roll = m_pController->VGetTargetRoll();
		m_pCamera->SetRoll(roll);

		AREAL pitch = m_pController->VGetTargetPitch();
		m_pCamera->SetPitch(pitch);

		m_pCamera->VUpdate(deltaMilliseconds);

		m_pController->VSetTargetDirection(m_pCamera->GetDir());
	}
}

AVOID PlayerView::VRender(Renderer *pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
	//Clean back buffer render target and depth stencil
	//ALIGN16 AREAL32 bgColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f }; //color
	AREAL32 bg[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	ClearRenderTargetView(bg, NULL);
	ClearDepthStencilView(true, false, 1.0f, 1, NULL);

	//render widgets
	for (Widgets::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
	{
		(*it)->VRender(pRenderer, r64Time, r64ElapsedTime);
	}
}

ABOOL PlayerView::VMsgProc(SystemMessage * msg)
{
	//Use reverse_iterator as widgets process messages
	//from top of screen to bottom
	for (Widgets::reverse_iterator it = m_widgets.rbegin(); it != m_widgets.rend(); it++)
	{
		if ((*it)->VMsgProc(msg))
			return true;
	}

	//no widget processed a message, so give controllers a try
	ABOOL res = false;
	switch(msg->m_type)
	{

	case SMT_KeyDown:
		if (m_pKeyboardHandler)
		{
			KeyDownMessage * pMsg = static_cast<KeyDownMessage *>(msg);
			res = m_pKeyboardHandler->VOnKeyDown(pMsg->m_key);
		}
		break;

	case SMT_KeyUp:
		if (m_pKeyboardHandler)
		{
			KeyUpMessage * pMsg = static_cast<KeyUpMessage *>(msg);
			res = m_pKeyboardHandler->VOnKeyUp(pMsg->m_key);
		}
		break;

	case SMT_MouseMove:
		if (m_pMouseHandler)
		{
			MouseMoveMessage * pMsg = static_cast<MouseMoveMessage *>(msg);
			res = m_pMouseHandler->VOnMouseMove(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_LMouseDown:
		if (m_pMouseHandler)
		{
			LMouseDownMessage * pMsg = static_cast<LMouseDownMessage *>(msg);
			res = m_pMouseHandler->VOnLButtonDown(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_LMouseUp:
		if (m_pMouseHandler)
		{
			LMouseUpMessage * pMsg = static_cast<LMouseUpMessage *>(msg);
			res = m_pMouseHandler->VOnLButtonUp(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_RMouseDown:
		if (m_pMouseHandler)
		{
			RMouseDownMessage * pMsg = static_cast<RMouseDownMessage *>(msg);
			res = m_pMouseHandler->VOnRButtonDown(pMsg->m_posx, pMsg->m_posy);
		}
		break;

	case SMT_RMouseUp:
		if (m_pMouseHandler)
		{
			RMouseUpMessage * pMsg = static_cast<RMouseUpMessage *>(msg);
			res = m_pMouseHandler->VOnRButtonUp(pMsg->m_posx, pMsg->m_posy);
		}
		break;
	};

	return res;
}

AVOID PlayerView::VSetCamera(CameraPtr pCamera)
{
	m_pCamera = pCamera;
	m_bHasCamera = true;
}

AVOID PlayerView::AddWidget(WidgetPtr pWidget)
{
	m_widgets.push_back(pWidget);
}

ABOOL PlayerView::RemoveWidget(WidgetPtr pWidget)
{
	for( Widgets::iterator it = m_widgets.begin(); it != m_widgets.end(); it++)
	{
		if ((*it) == pWidget)
		{
			m_widgets.erase(it);
			return true;
		}
	}
	
	return false;
	//assert(0 && "No such widget was found!!!");
}