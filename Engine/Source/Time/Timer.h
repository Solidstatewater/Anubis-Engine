#pragma once
//====================================================================================
//Timer.h : Define game timer
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

namespace Anubis
{
	class Timer
	{
	private:

		//Time of the last frame (in ticks)
		AINT64 m_i64StartTime;

		//Number of CPU ticks in millisecond
		AREAL m_r32TicksInMs;

		//Time taken for frame(in ms)
		AREAL m_r32FrameTime;

		//Time passed since game started
		AINT64	m_i64GameTime;

	public:

		/*** Constructor ***
			 Destructor
		***				 ***/
		Timer();
		~Timer() {}

		//Tick
		AVOID Tick();

		/***
			//Getters
						***/

		AREAL GetGameTime() const			{ return m_i64GameTime / m_r32TicksInMs; }
		AREAL GetGameTimeSeconds() const	{ return m_i64GameTime / m_r32TicksInMs / 1000.0; }
		AREAL GetDeltaTime() const			{ return m_r32FrameTime;}
		AREAL GetDeltaTimeSeconds() const	{ return m_r32FrameTime / 1000.0; }
	};
}; //Anubis