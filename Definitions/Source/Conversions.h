#pragma once
//====================================================================================
//Conversions.h
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

#include "Types.h"
#include "Conventions.h"
//#include "Casting.h"

namespace Anubis
{
	///////////////////////////////////////////
	//Methods one may need for cross-platform
	///////////////////////////////////////////

	AINLINE	AUINT16 swap16( AUINT16 n )
	{
		return ((n & 0x00FF) << 8) |
			((n & 0xFF00) >> 8);
	}

	AINLINE	AUINT32	swap32( AUINT32 n )
	{
		return ((n & 0x000000FF) << 24) |
			((n & 0x0000FF00) << 8)	|
			((n & 0x00FF0000) >> 8) |
			((n & 0xFF000000) >> 24);
	}

	//AINLINE AREAL32 swapReal32( AREAL32 f )
	//{
	//	AUINT32 i = safe_cast(f, AUINT32);
	//	i = swap32(i);

	//	f = safe_cast(i, AREAL32);
	//	return f;
	//}
}; //Anubis