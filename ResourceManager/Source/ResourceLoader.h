#pragma once
//====================================================================================
//ResourceLoader.h
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

#include "Resource.h"

namespace Anubis
{
	class IResourceLoader
	{
	public:

		//constructor 
		IResourceLoader() {}

		AVIRTUAL ASTRING VGetPattern() = 0;
		AVIRTUAL ABOOL VUseRawFile() = 0;
		AVIRTUAL ABOOL VNeedFile() = 0;
		AVIRTUAL AUINT32 VGetLoadedResourceSize(ACHAR* pRawBuffer, AUINT32 uRawSize) = 0;
		AVIRTUAL ABOOL VLoadResource(ACHAR* pRawBuffer, AUINT32 uRawSize, std::shared_ptr<ResHandle> pHandle) = 0;
	};

	class DefaultResourceLoader : public IResourceLoader
	{
	public:
		AVIRTUAL ABOOL VUseRawFile() { return true; }
		AVIRTUAL ABOOL VNeedFile() { return false; }
		AVIRTUAL AUINT32 VGetLoadedResourceSize(CHAR* pRawBuffer, AUINT32 uRawSize)
		{
			return uRawSize; 
		}
		AVIRTUAL ABOOL VLoadResource(CHAR* pRawBuffer, AUINT32 uRawSize, std::shared_ptr<ResHandle> pHandle) { return true; }
		AVIRTUAL ASTRING VGetPattern() { return "*"; }
	};

}; //Anubis