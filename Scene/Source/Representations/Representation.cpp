//====================================================================================
//Representation.h
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

#include "Scene_pch.h"
#include "Representation.h"
#include "../Scene.h"

#include "../../ResourceManager/Source/Resource.h"
#include "../../ResourceManager/Source/ResourceCache.h"
#include "../../Renderer/Source/Resources/MeshResource.h"

using namespace Anubis;

EntityRepresentation::EntityRepresentation(EntityRepresentationId id, const ASTRING resourceMeshName)
	: m_id(id), m_r64LastUpdate(0.0f)
{
	m_f3Size = float3(0.0f, 0.0f, 0.0f);
	m_currentWorldMatrix = Mat4x4::Identity();

	Resource meshResource(resourceMeshName);
	shared_ptr<ResHandle> pMeshes = Anubis::SafeGetHandle(&meshResource);
	std::shared_ptr<MeshResourceExtraData> pData = static_pointer_cast<MeshResourceExtraData>(pMeshes->GetExtra());

	AUINT32 uNumMeshes = pData->GetNumMeshes();
	for (AUINT32 i = 0; i < uNumMeshes; i++)
	{
		m_meshes.push_back(pData->m_pMeshes[i]);
	}
}
EntityRepresentation::~EntityRepresentation()
{
	//for (EntityRepresentation * pRepr : m_children)
	for (Representations::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		EntityRepresentation* pRepr = (*it);
		m_children.erase(it);
		delete pRepr;
	}
}

//Add and remove children
AVOID EntityRepresentation::VAddChild(EntityRepresentation* pRepr)
{
	//add child to the list
	m_children.push_back(pRepr);
}

AVOID EntityRepresentation::VRemoveChild(EntityRepresentationId id)
{
}

EntityRepresentation* EntityRepresentation::VFindChild(EntityRepresentationId id)
{
	//compare IDs and return 
	//representation if equal
	if (m_id == id)	return this;

	//check out children
//	for (EntityRepresentation* pRepr : m_children)
	for (Representations::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		EntityRepresentation* pRepr = (*it);
		EntityRepresentation* pResult = pRepr->VFindChild(id);

		if (pResult) return pResult;
	}

	//not found
	return NULL;
}

AVOID EntityRepresentation::VAddMesh(MeshPtr pMesh)
{
	m_meshes.push_back(pMesh);
}

AVOID EntityRepresentation::VAddLight(LightPtr pLight)
{
	m_lights.push_back(pLight);
}

AVOID EntityRepresentation::SetCurrentTransform(Mat4x4 & mat, AREAL64 r64CurrentTime)
{
	m_currentWorldMatrix = mat;
	m_r64LastUpdate = r64CurrentTime;
}

//put all low-level mesh data to the renderer
AVOID EntityRepresentation::VRender(Scene * pScene, Renderer * pRenderer, AREAL64 r64CurrentTime)
{
	//push parameteres to the scene
	VPushParameters(pScene, r64CurrentTime);

	//send rendering data to the renderer
	VSendRenderingData(pScene, pRenderer);

	//render all children
//	for (EntityRepresentation* pRepr : m_children)
	for (Representations::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		EntityRepresentation* pRepr = (*it);
		pRepr->VRender(pScene, pRenderer, r64CurrentTime);
	}

	//undo all changes made to the scene
	VPullParameters(pScene);
}

//pre-rendering modifications
AVOID EntityRepresentation::VPushParameters(Scene *pScene, AREAL64 r64CurrentTime)
{
	pScene->PushTransform(m_currentWorldMatrix);
}

//push data to the renderer
AVOID EntityRepresentation::VSendRenderingData(Scene * pScene, Renderer * pRenderer)
{
	//send all meshes to the renderer
	//for (MeshPtr pMesh : m_meshes)
	for (Meshes::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		MeshPtr pMesh = (*it);
		pMesh->SetWorldTransform(pScene->GetTransform());
		pRenderer->VPushMesh(&*pMesh);
	}

	//send all lights to the renderer
	//for (LightPtr pLight : m_lights)
	for (Lights::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		LightPtr pLight = (*it);
		pLight->SetWorldTransform(pScene->GetTransform());
		pRenderer->VPushLight(&*pLight);
	}
}

//undo any changes made to the scene
AVOID EntityRepresentation::VPullParameters(Scene * pScene)
{
	pScene->PullTransform();
}