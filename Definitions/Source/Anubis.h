#pragma once
//====================================================================================
//Anubis.h
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
#include "Error_Handling\Assert.h"

#include "Strings\HashedString.h"
#include "Strings\Operations.h"

#include "Defines.h"
#include "Types.h"
#include "Conventions.h"
#include "Conversions.h"
//#include "Casting.h"

namespace Anubis
{
	//Struct for storing OS message data
/*#ifdef WIN32
	struct AppMsg
	{
		HWND	m_hWnd;
		UINT	m_uMsg;
		WPARAM	m_wParam;
		LPARAM	m_lParam;
	};
#endif //WIN32 */
	enum SystemMessageType
	{
		SMT_Quit,
		SMT_KeyDown,
		SMT_KeyUp,
		SMT_LMouseDown,
		SMT_LMouseUp,
		SMT_RMouseDown,
		SMT_RMouseUp,
		SMT_MouseMove,
	};

	struct SystemMessage
	{
		SystemMessageType	m_type;
	};

	struct QuitMessage : public SystemMessage
	{
		explicit QuitMessage()
		{
			m_type = SMT_Quit;
		}
	};

	struct KeyboardMessage : public SystemMessage
	{
		ACHAR m_key;

		KeyboardMessage(const CHAR key)
		{
			m_key = key;
		}
	};

	struct KeyDownMessage : public KeyboardMessage
	{
		explicit KeyDownMessage(const ACHAR key) : KeyboardMessage(key)
		{
			m_type = SMT_KeyDown;
		}
	};

	struct KeyUpMessage : public KeyboardMessage
	{
		explicit KeyUpMessage(const ACHAR key) : KeyboardMessage(key)
		{
			m_type = SMT_KeyUp;
		}
	};

	struct MouseMessage : public SystemMessage
	{
		AREAL	m_posx;
		AREAL	m_posy;

		MouseMessage(const AREAL x, const AREAL y) 
		{
			m_posx = x;
			m_posy = y;
		}
	};

	struct LMouseDownMessage : public MouseMessage
	{
		explicit LMouseDownMessage(const AREAL x, const AREAL y) : MouseMessage(x, y)
		{
			m_type =	SMT_LMouseDown;
		}
	};

	struct LMouseUpMessage : public MouseMessage
	{
		explicit LMouseUpMessage(const AREAL x, const AREAL y) : MouseMessage(x, y)
		{
			m_type =	SMT_LMouseUp;
		}
	};

	struct RMouseDownMessage : public MouseMessage
	{
		explicit RMouseDownMessage(const AREAL x, const AREAL y) : MouseMessage(x, y)
		{
			m_type =	SMT_RMouseDown;
		}
	};

	struct RMouseUpMessage : public MouseMessage
	{
		explicit RMouseUpMessage(const AREAL x, const AREAL y) : MouseMessage(x, y)
		{
			m_type =	SMT_RMouseUp;
		}
	};

	struct MouseMoveMessage : public MouseMessage
	{
		explicit MouseMoveMessage(const AREAL x, const AREAL y) : MouseMessage(x, y)
		{
			m_type =	SMT_MouseMove;
		}
	};

}; //Anubis

//#define SCREEN_WIDTH 1920
//#define SCREEN_HEIGHT 1080