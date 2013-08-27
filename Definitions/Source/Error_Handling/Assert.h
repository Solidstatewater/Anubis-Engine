//====================================================================================
//Assert.h : Asserts and other debugging declarations
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

#pragma once

#include <winerror.h>

#define ASSERTIONS_ENABLED 1

#define DebugBrake() __asm { int 3 }

#ifdef ASSERTIONS_ENABLED
	#define ASSERT(expr) \
		if (expr)	\
			{ }		\
		else \
		{ \
			/*reportAssertionFailure(#expr, __FILE__, __LINE__); \
			DebugBrake(); */ \
		} 
#else 
	#define ASSERT(expr)
#endif		 

#ifdef _DEBUG 
	#define Check(x) \
		OutputDebugStringA(x); \
		DebugBrake();
#else
	#define Check(x) \
		OutputDebugStringA(x)
#endif

#ifdef _DEBUG
	#define CheckHR(x) \
		switch (x) \
		{ \
			case S_OK: \
				break; \
			\
			case S_FALSE: \
				OutputDebugStringA("Completed without error, but only partial results were obtained. \
							If a buffer is not large enough to hold the information that is returned \
							to it, the returned information is often truncated to fit into the buffer \
							and S_FALSE is returned from the method."); \
				break; \
			\
			case E_FAIL: \
				OutputDebugStringA("Operation couldn't be performed.");\
				break; \
			\
			case E_INVALIDARG: \
				OutputDebugStringA("One of the arguments passed in was invalid.");\
				break; \
			\
			case E_NOINTERFACE: \
				OutputDebugStringA("The object searched for was not found.");\
				break; \
			\
			case E_OUTOFMEMORY: \
				OutputDebugStringA("A memory allocation attempt failed.");\
				break; \
			\
			case E_UNEXPECTED: \
				OutputDebugStringA("The target was not accessible, ot the engine was not in a state \
							where the function or method could be processed.");\
				break; \
			\
			case E_NOTIMPL: \
				OutputDebugStringA("Not implemented."); \
				break; \
			\
			default: \
				OutputDebugStringA("File not found."); \
				break; \
		};\
		DebugBrake(); 
#else

	#define CheckHR(x) {}

#endif

#define VALID(hr) \
	if (hr == S_OK) \
		return true; \
	else \
	{ \
		CheckHR(hr); \
		return false; \
	};