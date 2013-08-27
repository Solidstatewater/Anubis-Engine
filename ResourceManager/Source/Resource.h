#pragma once
//====================================================================================
//Resource.h
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

#include <algorithm>

namespace Anubis
{
	class Resource
	{
	public:
		ASTRING m_name;

	public:
		Resource(const ASTRING & name);
	};

	class IResourceExtraData
	{
	public:
		AVIRTUAL ASTRING VToString() = 0;
	};


	class ResourceCache;
	class ResHandle
	{
		friend class ResourceCache;

	protected:
		Resource	m_resource;
		ACHAR*		m_pBuffer;
		AUINT32		m_uSize;
		std::shared_ptr<IResourceExtraData> m_pExtra;
		
		//pointer to it's resource cache
		ResourceCache*	m_pResourceCache;

	public:
		ResHandle(Resource & resource, ACHAR* pBuffer, AUINT32 uSize, ResourceCache* pResCache);
		AVIRTUAL ~ResHandle();

		const ASTRING GetName() { return m_resource.m_name; }
		AUINT32 Size() const { return m_uSize; }
		ACHAR*  Buffer() const { return m_pBuffer; }
		ACHAR*	WritableBuffer() { return m_pBuffer; }

		std::shared_ptr<IResourceExtraData> GetExtra() { return m_pExtra; }
		void SetExtra(std::shared_ptr<IResourceExtraData> pExtra) { m_pExtra = pExtra; }
	};

}; //Anubis