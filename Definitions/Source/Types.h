//====================================================================================
//Types.h : Redeclare types
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

////////////////////////////////////////////////
//Include files
////////////////////////////////////////////////
#include <basetsd.h>
#include <string>
#include <vector>
#include <queue>
#include <list>

#include <wchar.h>
#include <tchar.h>

//typedef float		R32;
//typedef double		R64;

//typedef __int8		I8;
//typedef __int16		I16;
//typedef __int32		I32;
//typedef __int64		I64;
//typedef __int128	I128;

namespace Anubis
{
////////////////////////////////////////
//floating-point types
#define AREAL		float
#define AREAL32		float
#define	AREAL64		double

////////////////////////////////////////
//integer types
#define	AINT8		__int8
#define	AINT16		__int16
#define	AINT32		__int32
#define	AINT64		__int64

#define	ACHAR		char
#define AWCHAR		wchar_t
#define ATCHAR		TCHAR
#define	AUINT8		unsigned char
#define AUINT16		unsigned short
#define	AUINT32		unsigned int
#define	AUINT64		unsigned __int64

#define ALARGE_INT	LARGE_INTEGER

////////////////////////////////////////
//boolean
#define ABOOL		bool

////////////////////////////////////////
//strings
#define ASTRING		std::string
#define AWSTRING	std::wstring

////////////////////////////////////////
//containers
#define AVECTOR		std::vector
#define ALIST		std::list
#define AQUEUE		std::queue

////////////////////////////////////////
//other types
#define	AVOID		void
#define APVOID		void*
#define	AUINTPTR	UINT_PTR
#define	AINTPTR		INT_PTR
}; //Anubis