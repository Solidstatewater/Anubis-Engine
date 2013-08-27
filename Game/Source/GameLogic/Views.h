#pragma once
//====================================================================================
//Views.h
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

#include "Renderer.h"
#include "Messenger.h"
#include "Widget.h"

#include "../../Input/Source/Devices.h"
#include "../../Input/Source/Controllers.h" 

#include "Anubis.h"

namespace Anubis
{
	class IView
	{
	public:
		AVIRTUAL ~IView() {}

		AVIRTUAL ABOOL		VInit()												= 0;
		AVIRTUAL ABOOL		VRestore()											= 0;
		AVIRTUAL AVOID		VUpdate(AUINT32 const deltaMilliseconds)			= 0;
		AVIRTUAL AVOID		VRender(Renderer* pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime)	= 0;
		AVIRTUAL ABOOL		VMsgProc(SystemMessage * msg)						= 0;

		AVIRTUAL	AVOID	  VSetCamera(CameraPtr pCamera) = 0;
		AVIRTUAL	CameraPtr VGetCamera() const = 0;
		AVIRTUAL	ABOOL	  VHasCamera() const = 0;
	};

	class PlayerView : public IView
	{
	protected:
		IMouseHandler*		m_pMouseHandler;
		IKeyboardHandler*	m_pKeyboardHandler;  
		MovementController*	m_pController;

	public:
		PlayerView();
		AVIRTUAL ~PlayerView() {
			SAFE_DELETE(m_pController);
			}

		AVIRTUAL ABOOL		VInit();
		AVIRTUAL ABOOL		VRestore();
		AVIRTUAL AVOID		VUpdate(AUINT32 const deltaMilliseconds);
		AVIRTUAL AVOID		VRender(Renderer* pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime);
		AVIRTUAL ABOOL		VMsgProc(SystemMessage * msg);

	protected:
		/* ==================================
		//	Widgets
		=================================== */
		Widgets m_widgets;

		/* ==================================
		// Game Rendering
		=================================== */
		CameraPtr	m_pCamera;
		ABOOL		m_bHasCamera;

		AREAL32 bgColor[4];

	public:

		//Widgets
		AVIRTUAL AVOID AddWidget(WidgetPtr pWidget);
		AVIRTUAL ABOOL RemoveWidget(WidgetPtr pWidget);

		//Game Rendering
		AVIRTUAL AVOID VSetCamera(CameraPtr pCamera);
		AVIRTUAL CameraPtr VGetCamera() const { return m_pCamera; }
		AVIRTUAL ABOOL VHasCamera() const { return m_bHasCamera; }

		//Mutators
		AVOID SetMouseHandler(IMouseHandler* pHandler)		 { m_pMouseHandler = pHandler; }
		AVOID SetKeyboardHandler(IKeyboardHandler* pHandler) { m_pKeyboardHandler = pHandler; }
		AVOID SetController(MovementController* pController) { m_pController = pController; }
	};

	//typedefs
	typedef shared_ptr<IView>	ViewPtr;
	typedef vector<ViewPtr>		Views;

}; //Anubis