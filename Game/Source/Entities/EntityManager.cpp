//====================================================================================
//EntityManager.cpp
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

#include "Game_pch.h"
#include "EntityManager.h"

using namespace Anubis;

//Return entity by its id
EntityPtr EntityManager::VGetEntity(const EntityId id)
{
	//iterate the list and extract our entity
	for (auto ent = m_entities.begin(); ent != m_entities.end(); ent++)
		//the entity with such id found
			if ((*ent)->GetId() == id)
				return (*ent);

	ASSERT(0 && "No entity with such Id!");
	return nullptr;
}

//Add entity to the manager
AVOID EntityManager::VAddEntity(EntityPtr pEntity)
{
	//just insert it to the list
	m_entities.push_back(pEntity);
}

//Remove entity from the manager
ABOOL EntityManager::VRemoveEntity(const EntityId id)
{
	//iterate the list and extract our entity
	auto ent = m_entities.begin();
	for (; ent != m_entities.end(); ent++)
		//the entity with such id found
			if ((*ent)->GetId() == id)
				break;

	if (ent != m_entities.end())
	{
		m_entities.erase(ent);
		return true;
	}

	return false;
}

AVOID EntityManager::VUpdate(const AREAL64 r64Time, const AREAL64 r64ElapsedTime)
{
	for (auto ent = m_entities.begin(); ent != m_entities.end(); ent++)
	{
		(*ent)->VUpdate(r64Time, r64ElapsedTime);
	}
}

//Update entity transform matrix
AVOID EntityManager::VUpdateEntity(const EntityId id, Mat4x4 const & transform, AREAL64 r64CurrentTime)
{
	//iterate the list and extract our entity
	for (auto ent = m_entities.begin(); ent != m_entities.end(); ent++)
		//the entity with such id found
			if ((*ent)->GetId() == id)
			{
				(*ent)->SetCurrentTransform(transform, r64CurrentTime);
				return;
			}

	ASSERT(0 && "No entity with such Id!");
}