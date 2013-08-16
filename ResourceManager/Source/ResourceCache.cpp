//====================================================================================
//ResourceCache.cpp
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

#include "ResourceManager_pch.h"
#include "ResourceCache.h"
#include "ResourceLoader.h"

using namespace Anubis;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	Global pointer to the resource cache
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
ASTATIC ResourceCache * g_pCache = NULL;

ResourceCache::ResourceCache(const AUINT32 uSizeInMb, IResourceFile* pFile)
{
	m_uCacheSize = uSizeInMb * 1024 * 1024; //size in bytes
	m_uAllocated = 0;

	m_pFile = pFile;

	g_pCache = this;
}

ResourceCache * const ResourceCache::Get()
{
	return g_pCache;
}


ResourceCache::~ResourceCache()
{
	while (!m_lru.empty())
	{
		FreeOneResource();
	}
	SAFE_DELETE(m_pFile);
}

ABOOL ResourceCache::Init()
{
	ABOOL bResult = false;
	if (m_pFile->VOpen())
	{
		RegisterLoader(shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
		bResult = true;
	}

	return bResult;
}

shared_ptr<ResHandle> ResourceCache::GetHandle(Resource *pResource)
{
	shared_ptr<ResHandle> pHandle(Find(pResource));
	if (!pHandle)
	{
		pHandle = Load(pResource);
	}
	else
	{
		Update(pHandle);
	}

	return pHandle;
}

shared_ptr<ResHandle>  ResourceCache::Load(Resource *pResource)
{
	shared_ptr<IResourceLoader> pLoader;
	shared_ptr<ResHandle> pHandle;

	for (ResourceLoaders::iterator it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it)
	{
		shared_ptr<IResourceLoader> pCurLoader = (*it);
		if (WildcardMatch(pCurLoader->VGetPattern().c_str(), pResource->m_name.c_str()))
		{
			pLoader = pCurLoader;
			break;
		}
	}

	if (!pLoader)
	{
		assert(0 && "No loader is found for such a resource!");
		return pHandle;
	}

	AUINT32 uRawSize = m_pFile->VGetRawResourceSize(*pResource);
	ACHAR* pRawBuffer = pLoader->VUseRawFile() ? Allocate(uRawSize) : new ACHAR[uRawSize];

	if (!pRawBuffer)
	{
		//our resource cache ran out all memory
		return shared_ptr<ResHandle>();
	}

	m_pFile->VGetRawResource(*pResource, pRawBuffer);
	ACHAR* pBuffer = nullptr;
	AUINT32 uSize = 0;

	if (pLoader->VUseRawFile())
	{
		pBuffer = pRawBuffer;
		pHandle = shared_ptr<ResHandle>(new ResHandle(*pResource, pBuffer, uRawSize, this));
	}
	else
	{
		uSize = pLoader->VGetLoadedResourceSize(pRawBuffer, uRawSize);
		pBuffer = Allocate(uSize);
		if (!pRawBuffer || !pBuffer)
		{
			//our resource cache ran out all memory
			return shared_ptr<ResHandle>();
		}

		pHandle = shared_ptr<ResHandle>(new ResHandle(*pResource, pBuffer, uSize, this));
		ABOOL bSuccess;
		if (pLoader->VNeedFile())
		{
			bSuccess = pLoader->VLoadResource(const_cast<char*>(pResource->m_name.c_str()), uRawSize, pHandle);
		}
		else
		{
			bSuccess = pLoader->VLoadResource(pRawBuffer, uRawSize, pHandle);
		}
		SAFE_DELETE_ARRAY(pRawBuffer);

		if (!bSuccess)
		{
			//our resource cache ran out all memory
			return shared_ptr<ResHandle>();
		}
	}

	//put resource handle to cache lists
	if (pHandle)
	{
		m_lru.push_front(pHandle);
		m_resources[pResource->m_name] = pHandle;
	}

	return pHandle;
} 

ACHAR* ResourceCache::Allocate(AUINT32 uSize)
{
	if (!MakeRoom(uSize))
		return nullptr;

	ACHAR* pMemory = new ACHAR[uSize];
	if (pMemory)
		m_uAllocated += uSize;

	return pMemory;
}

ABOOL ResourceCache::MakeRoom(AUINT32 uSize)
{
	if (uSize > m_uCacheSize)
	{
		return false;
	}

	while (uSize > (m_uCacheSize - m_uAllocated))
	{
		//cache is empty and it's still not enough room to allocated the resource
		if (m_lru.empty()) 
			return false;

		FreeOneResource();
	}

	return true;
}

AVOID ResourceCache::FreeOneResource()
{
	ResHandleList::iterator freeMe = m_lru.end();
	freeMe--;

	shared_ptr<ResHandle> pHandle = *freeMe;

	m_lru.pop_back();
	m_resources.erase(pHandle->m_resource.m_name);
}

AVOID ResourceCache::MemoryHasBeenFreed(AUINT32 uSize)
{
	m_uAllocated -= uSize;
}

AINT32 ResourceCache::Preload(const ASTRING pattern, AVOID (*progressCallback)(AINT32, ABOOL &))
{
	if (m_pFile == NULL)
		return 0;

	AINT32 iNumFiles = m_pFile->VGetNumResources();
	AINT32 iNumLoaded = 0;
	ABOOL bCancel = false;

	for (AINT32 i = 0; i < iNumFiles; ++i)
	{
		Resource resource(m_pFile->VGetResourceName(i));

		if (WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
		{
			shared_ptr<ResHandle> pHandle = GetHandle(&resource);
			++iNumLoaded;
		}

		if (progressCallback)
		{
			progressCallback(i* 100.0f / (AREAL)iNumFiles, bCancel);
		}
	}

	return iNumLoaded;
}

AVOID ResourceCache::RegisterLoader(std::shared_ptr<IResourceLoader> pLoader)
{
	m_resourceLoaders.push_front(pLoader);
}

AVOID ResourceCache::Update(std::shared_ptr<ResHandle> pHandle)
{
	m_lru.remove(pHandle);
	m_lru.push_front(pHandle);
}

std::shared_ptr<ResHandle> ResourceCache::Find(Resource * r)
{
	ResHandleMap::iterator i = m_resources.find(r->m_name);
	if (i==m_resources.end())
		return shared_ptr<ResHandle>();

	return i->second;
}

/////////////////////////////////////////////////
//Global resource cache methods
/////////////////////////////////////////////////
std::shared_ptr<ResHandle> Anubis::SafeGetHandle(Resource *pResource)
{
	//assert that we have valid pointer to the messenger
	ASSERT(ResourceCache::Get());

	//execute internal methods
	return ResourceCache::Get()->GetHandle(pResource);
}