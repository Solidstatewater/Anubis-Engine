#pragma once
//====================================================================================
//MeshResource.h
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

#include "../../ResourceManager/Source/Resource.h"
#include "../../ResourceManager/Source/ResourceLoader.h"

#include "../../Renderer/Source/Other/Material.h"
#include "../Meshes/Meshes.h"

#include <assimp/Importer.hpp>	//c++ importer interface
#include <assimp/scene.h>		//output data structure
#include <assimp/postprocess.h>	//post processing flags

namespace Anubis
{
	class VertexBuffer;
	class IndexBuffer;
	class MeshResourceExtraData : public IResourceExtraData
	{
		friend class ObjResourceLoader;

	public: 	
		MeshResourceExtraData();
		AVIRTUAL ~MeshResourceExtraData() { }
		AVIRTUAL ASTRING VToString() { return "MeshResourceExtraData"; }

		AUINT32 GetNumMeshes() const { return m_uNumMeshes; }
		Vec 	GetExtents()	const { return m_vExtents; }

	protected:
		AUINT32 m_uNumMeshes;
		Vec		m_vExtents;

		//make an exception, it's just a lot easier this way
	public:
		//IndexedMesh*	m_pMeshes;
		std::vector<shared_ptr<IndexedMesh> > m_pMeshes;
	};

	class ObjResourceLoader : public IResourceLoader
	{
	public:
		ObjResourceLoader();

		AVIRTUAL ABOOL VUseRawFile() { return false; }
		AVIRTUAL ABOOL VNeedFile() { return true; }
		AVIRTUAL ABOOL VDiscardRawBufferAfterLoad() { return true; }
		AVIRTUAL AUINT32 VGetLoadedResourceSize(ACHAR *pRawBuffer, AUINT32 uRawSize);
		AVIRTUAL ABOOL VLoadResource(ACHAR *pRawBuffer, AUINT32 uRawSize, std::shared_ptr<ResHandle> pHandle);
		AVIRTUAL ASTRING VGetPattern() { return "*.obj"; }

	protected:
		//ABOOL LoadMtl(NameMaterialMap & map, AWSTRING fileName)
		ABOOL ParseObj(ACHAR *pObjStream, size_t bufferLength, shared_ptr<ResHandle> pHandle);
		//AVOID UnsupportedtoJPG(ACHAR * pStr);
	
	protected:
		const aiScene*	m_pScene;
		Assimp::Importer m_Importer;
	};
}; 