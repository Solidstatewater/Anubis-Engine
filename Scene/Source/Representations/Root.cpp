//====================================================================================
//Root.cpp
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

#include "Scene_pch.h"
#include "Root.h"

using namespace Anubis;

//constructor
RootRepresentation::RootRepresentation() : EntityRepresentation(0)
{
}

//Add and remove children
AVOID RootRepresentation::VAddChild(EntityRepresentation* pRepr, RenderPass pass,
									EntityRepresentationId parentId)
{
	//if no parent for this representation than just 
	//add it to the right pass
	if (!parentId)
		m_passes[pass].push_back(pRepr);
	else
	{
		if (EntityRepresentation * pParent = FindChild(parentId, pass))
		{
			pParent->VAddChild(pRepr);
		}
		else
		{
			assert(0 && "Parent with such Id doesn't exists or lies in the other pass!");
		}
	}
}
AVOID RootRepresentation::VRemoveChild(EntityRepresentationId id)
{
}

EntityRepresentation* RootRepresentation::VFindChild(EntityRepresentationId id)
{
	for (int pass = 0; pass < NUM_RENDER_PASSES; pass++)
	{
		if (EntityRepresentation* pResult = FindChild(id, (RenderPass)pass))
		{
			return pResult;
		}
	}

	//not found
	return NULL;
}

EntityRepresentation* RootRepresentation::FindChild(EntityRepresentationId id, RenderPass pass)
{
	//check out all children
	//for (EntityRepresentation* pRepr : m_passes[pass])
	for (Representations::iterator it = m_passes[pass].begin(); it != m_passes[pass].end(); it++)
	{
		EntityRepresentation* pRepr = (*it);
		if (EntityRepresentation* pResult = pRepr->VFindChild(id))
		{
			return pResult;
		}
	}

	//not found
	return NULL;
}

//put all low-level mesh data to the renderer
AVOID RootRepresentation::VRender(Scene* pScene, Renderer * pRenderer, AREAL64 r64CurrentTime)
{
	VPushParameters(pScene, r64CurrentTime);
	VSendRenderingData(pScene, pRenderer);

	//render all passes
	for (int i  = 0; i < NUM_RENDER_PASSES; i++)
	{
		//render all representations from the given pass
		for (Representations::iterator it = m_passes[i].begin(); it != m_passes[i].end(); it++)
		{
			EntityRepresentation* pRepr = (*it);
			pRepr->VRender(pScene, pRenderer, r64CurrentTime);
		}
	}

	VPullParameters(pScene);
}