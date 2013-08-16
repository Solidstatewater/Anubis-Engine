//====================================================================================
//Timer.cpp : Implement game timer
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

#include "Engine_pch.h"
#include "Timer.h"

using namespace Anubis;

Timer::Timer() : m_i64GameTime(0)
{
	//get CPU frequency 
	AINT64 freq;
	QueryPerformanceFrequency((ALARGE_INT*)&freq);

	//calculate number of ticks in one ms
	m_r32TicksInMs = (AREAL)( (AREAL)freq / 1000.0);

	QueryPerformanceCounter((ALARGE_INT*)&m_i64StartTime);
}

AVOID Timer::Tick()
{
	AINT64 i64CurrTime;
	AREAL r32DeltaTime;

	//get current time
	QueryPerformanceCounter((ALARGE_INT*)&i64CurrTime);

	//calculate delta
	r32DeltaTime = (AREAL)(i64CurrTime - m_i64StartTime);

	//convert to milliseconds
	m_r32FrameTime = r32DeltaTime / m_r32TicksInMs;

	//update time of previous frame
	m_i64StartTime = i64CurrTime;

	//sum up total time
	m_i64GameTime += r32DeltaTime;
}