#pragma once
//====================================================================================
//ResourceCache.h
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
#include "ResourceLoader.h"
#include "ResourceFile.h"

namespace Anubis
{
	class ResourceCache
	{

		friend class ResHandle;

		//Global resource cache methods
		friend std::shared_ptr<ResHandle> SafeGetHandle(Resource *pResource);

	protected:
		IResourceFile* m_pFile;

		typedef std::list< std::shared_ptr<ResHandle> > ResHandleList;
		typedef std::map<ASTRING, std::shared_ptr<ResHandle> > ResHandleMap;
		typedef std::list< std::shared_ptr<IResourceLoader> > ResourceLoaders;

		ResHandleList m_lru; //leat-recetly used
		ResHandleMap m_resources; //fast resource look-up
		ResourceLoaders m_resourceLoaders;

		//////////////////////////////////////////////
		//Chache parameteres
		AUINT32 m_uCacheSize; //size of resource cahce
		AUINT32 m_uAllocated; //size of allocated resources

		std::shared_ptr<ResHandle> Find(Resource* pResource);
		VOID Update(std::shared_ptr<ResHandle> pHandle);
		std::shared_ptr<ResHandle> Load(Resource *pResource);
		AVOID Free(std::shared_ptr<ResHandle>  pFreeMe);

		ABOOL MakeRoom(AUINT32 uSize);
		ACHAR* Allocate(AUINT32 uSize);
		AVOID FreeOneResource();
		AVOID MemoryHasBeenFreed(AUINT32 uSize);

	public:

		ASTATIC ResourceCache * const Get();

		ResourceCache(const AUINT32 sizeInMb, IResourceFile* pFile);
		~ResourceCache();

		ABOOL Init();
		AVOID RegisterLoader(std::shared_ptr<IResourceLoader> pLoader);

		std::shared_ptr<ResHandle> GetHandle(Resource *pResource);
		AINT32 Preload(const ASTRING pattern, AVOID (*progressCallback)(AINT32, ABOOL &));
		AVOID Flush();
	};

}; //Anubis