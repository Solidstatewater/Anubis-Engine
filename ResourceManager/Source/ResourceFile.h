#pragma once
//====================================================================================
//ResourceFile.h
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

#include "Resource.h"

namespace Anubis
{
	class IResourceFile
	{
	public:
		//need virtual destructor
		AVIRTUAL ~IResourceFile() {}

		AVIRTUAL ABOOL VOpen() = 0;
		AVIRTUAL AINT32 VGetRawResourceSize(const Resource & resource) = 0;
		AVIRTUAL AINT32 VGetRawResource(const Resource & resource, ACHAR* pBuffer) = 0;
		AVIRTUAL AINT32 VGetNumResources() const = 0;
		AVIRTUAL ASTRING VGetResourceName(const AINT32 num) const = 0;
	};

	typedef std::map<ASTRING, AINT32> ContentMap;
	class ResourceDir : public IResourceFile
	{
	protected:

		std::vector<WIN32_FIND_DATA> m_resourcesData;
		ContentMap m_directoryContent;
		AWSTRING m_directory;

	protected:
		AVOID ReadResourceDirectory(const AWSTRING & pattern);

	public:

		//constructor
		ResourceDir(const AWSTRING & dir);

		ABOOL VOpen();
		AVIRTUAL AINT32 VGetRawResourceSize(const Resource & resource);
		AVIRTUAL AINT32 VGetRawResource(const Resource & resource, ACHAR* pBuffer);
		AVIRTUAL AINT32 VGetNumResources() const;
		AVIRTUAL ASTRING VGetResourceName(const AINT32 num) const;

		AINT32 Find(const ASTRING & name);
	};

}; //Anubis