//====================================================================================
//Scene.cpp
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
#include "Scene.h"

#include "../../Scene/Source/Representations/StaticRepresentation.h"

#include "Allocator.h"

using namespace Anubis;

Scene::Scene()
{
	//initialize root node
	//RootRepresentation * pRepr = allocator._new<RootRepresentation>();
	//m_pRoot = make_shared<RootRepresentation>(*pRepr);
	Allocator allocator;
	APVOID pMemory = allocator.allocateAligned(sizeof(RootRepresentation), 16);
	m_pRoot = new (pMemory) RootRepresentation();

	m_r64FixedTimeStep = 0.0333;
}

Scene::~Scene()
{
	//delete m_pRoot;

	while (!m_staticObjects.empty())
	{
		StaticRepresentation* pRepr = m_staticObjects.back();
		m_staticObjects.pop_back();

		delete pRepr;
	}
}

/***	====	Initialization	====	***/
ABOOL Scene::VInitialize()
{
	return true;
}

/***	====	Updating	====	***/
AVOID Scene::VUpdate(AREAL64 r64Time, AREAL64 r64ElapsedTime)
{
}

/***	====	Rendering	====	***/
AVOID Scene::VRender(Renderer * pRenderer, AREAL64 r64CurrentTime)
{
	for (StaticObjects::iterator it = m_staticObjects.begin(); it != m_staticObjects.end(); it++)
	{
		(*it)->VRender(this, pRenderer, r64CurrentTime);
	}

	m_pRoot->VRender(this, pRenderer, r64CurrentTime);
}

/***	====	Representations management	====	***/
EntityRepresentation* Scene::GetRepresentation(EntityRepresentationId id)
{
	return m_pRoot->VFindChild(id);
}

ABOOL Scene::RemoveRepresentation(EntityRepresentationId id)
{
	return false;
}

AVOID Scene::AddRepresentation(EntityRepresentation* pRepresentation,
			RenderPass pass, EntityRepresentationId parentId)
{
	m_pRoot->VAddChild(pRepresentation, pass, parentId);
}

AVOID Scene::AddStaticObject(StaticRepresentation* pRepr)
{
	m_staticObjects.push_back(pRepr);
}

//update current world transform
AVOID Scene::PushTransform(const Mat4x4 & mat)
{
	m_transforms.PushTransform(mat);
}

Mat4x4 Scene::GetTransform() const
{
	return m_transforms.Get();
}

//remove last transform
AVOID Scene::PullTransform()
{
	m_transforms.Pop();
}