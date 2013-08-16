#pragma once
//====================================================================================
//Math_pch.h : precompiled header
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

#include <vector>

#include <Anubis.h>

#include "Vectors.h"
#include "Math.h"

#include <assert.h>

using namespace std;

namespace Anubis
{
	/*
	extern Color g_White;
	extern Color g_Black;
	extern Color g_Cyan;
	extern Color g_Red;
	extern Color g_Green;
	extern Color g_Blue;
	extern Color g_Yellow;
	extern Color g_Gray40;
	extern Color g_Gray25;
	extern Color g_Gray65;
	extern Color g_Transparent; */

	extern float3 g_up;
	extern float3 g_right;
	extern float3 g_left;
	extern float3 g_forward;
	extern float3 g_back;
	extern float3 g_camUp;	  //up direction for camera

	extern Vec g_up4;
	extern Vec g_right4;
	extern Vec g_left4;
	extern Vec g_forward4;
	extern Vec g_back4;
	extern Vec g_camUp4;    //up direction for camera

}; //Anubis