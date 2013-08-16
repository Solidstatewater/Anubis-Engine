#pragma once
//====================================================================================
//devices.h : input devices
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

#include "Defines.h"
#include "../../Math/Source/Math.h"

// ================= Mouse ================= //
class IMouseHandler
{
public:
	//left mouse button
	AVIRTUAL ABOOL VOnLButtonDown(const AREAL x, const AREAL y)	= 0;
	AVIRTUAL ABOOL VOnLButtonUp(const AREAL x, const AREAL y)	= 0;

	//right mouse button
	AVIRTUAL ABOOL VOnRButtonDown(const AREAL x, const AREAL y)	= 0;
	AVIRTUAL ABOOL VOnRButtonUp(const AREAL x, const AREAL y)	= 0;

	//mouse move
	AVIRTUAL ABOOL VOnMouseMove(const AREAL x, const AREAL y)	= 0;
};

// ================= Keyboard ================= //

//store pressed key
typedef AUINT8 Key;

class IKeyboardHandler
{
public:
	AVIRTUAL ABOOL VOnKeyDown(Key const key)	= 0;
	AVIRTUAL ABOOL VOnKeyUp(Key const key)		= 0;
};

// ================= Joystick ================= //