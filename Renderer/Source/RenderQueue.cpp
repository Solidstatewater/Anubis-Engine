//====================================================================================
//RenderQueue.cpp
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

#include "Renderer_pch.h"
#include "RenderQueue.h"

using namespace Anubis;

RenderQueue::RenderQueue() : m_numMeshes(0), m_cur(0)
{
}

AVOID RenderQueue::VAdd(Mesh * pMesh)
{
	//push mesh pointer to the queue
	m_queue.push_back(pMesh);

	//increment counter
	m_numMeshes++;
}

AVOID RenderQueue::VSort()
{
}

AVOID RenderQueue::Reset()
{
	m_currMesh = m_queue.begin();
}

AVOID RenderQueue::Clear()
{
	while (!m_queue.empty())
	{
		m_queue.pop_back();
	}
}

Mesh* RenderQueue::Next()
{
	if (m_currMesh == m_queue.end())	return NULL;

	Mesh* pMesh = (*m_currMesh);
	m_currMesh++;
	return pMesh;
}

Mesh* RenderQueue::NextWithDeletion()
{
	//we just rendered all meshes in the queue
	//if (m_cur == (m_numMeshes - 1))	return NULL;
	if (m_queue.empty())	return NULL;

	//return m_queue[m_cur++];
	Mesh * pMesh = m_queue.back();
	m_queue.pop_back();

	return pMesh;
}