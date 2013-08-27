#pragma once
//====================================================================================
//EntityManager.h
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

#include "Entity.h"
#include "EntityResource.h"

namespace Anubis
{
	//======================================//
	//		Entity Manager Interface        //
	//										//
	//	Manages, sort and updates all game	//
	//	entities.							//
	//======================================//
	class IEntityManager
	{
	public:
		//Return entity by its id
		AVIRTUAL EntityPtr	VGetEntity(const EntityId id) = 0;

		//Add entity to the manager
		AVIRTUAL AVOID		VAddEntity(EntityPtr pEntity) = 0;

		//Remove entity from the manager
		AVIRTUAL ABOOL		VRemoveEntity(const EntityId id) = 0;

		//Update entities
		AVIRTUAL AVOID		VUpdate(const AREAL64 r64Time, const AREAL64 r64ElapsedTime) = 0;

		//Update entity transform matrix
		AVIRTUAL AVOID		VUpdateEntity(const EntityId id, Mat4x4 const & transform, AREAL64 r64CurrentTime) = 0;
	};

	////////////////////////////////////////
	//Implementation of Entity Manager
	////////////////////////////////////////
	class EntityManager : public IEntityManager
	{
	private:

		//List of entities 
		typedef vector<EntityPtr>	Entities;
		Entities	m_entities;

		AUINT32		m_count; //number of entities in the list

	public:

		///////////////////////////////////////
		/*** IEntityManager Implementation ***/
		//									 //
		///////////////////////////////////////

		//Return entity by its id
		AVIRTUAL EntityPtr	VGetEntity(const EntityId id);

		//Add entity to the manager
		AVIRTUAL AVOID		VAddEntity(EntityPtr pEntity);

		//Remove entity from the manager
		AVIRTUAL ABOOL		VRemoveEntity(const EntityId id);

		//Update entities
		AVIRTUAL AVOID		VUpdate(const AREAL64 r64Time, const AREAL64 r64ElapsedTime);

		//Update entity transform matrix
		AVIRTUAL AVOID		VUpdateEntity(const EntityId id, Mat4x4 const & transform, AREAL64 r64CurrentTime);
	};

}; //Anubis