#pragma once
//====================================================================================
//Entity.h : Define Entity class
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

#include "../../Math/Source/Mat4x4.h"
#include "Representations\Representation.h"

namespace Anubis
{
	//Unique identifier for each game actor
	typedef AUINT32	EntityId;

	class IPhysics;
	class Entity
	{
	protected:
		//unique id for each entity
		EntityId	m_id;

		//entity transformation matrix
		Mat4x4		m_prevWorldMatrix;
		Mat4x4		m_currentWorldMatrix;

		//pointer to its representation
		EntityRepresentation *	m_pRepresentation;

	public:
		//constructor
		Entity(EntityId id) : m_id(id), m_prevWorldMatrix(Mat4x4::Identity()), m_currentWorldMatrix(Mat4x4::Identity()) 
		{
			m_pRepresentation = nullptr;
		}
		/***** Initializing *****/
		//AVIRTUAL EntityRepresentationPtr VCreateRepresentation(Scene * pScene) = 0;

		//create its physics model
		//AVIRTUAL AVOID VCreatePhysicalBody(IPhysics * pPhysics) = 0;

		/***** Accessors *****/
		EntityId	GetId() const { return m_id; } //return entity id
		Mat4x4		GetGurrentTransform() const { return m_currentWorldMatrix; }
		EntityRepresentation * GetRepresentation() const { return m_pRepresentation; }

		/***** Mutators *****/
		AVOID		SetRepresentation(EntityRepresentation* pRep)	{ m_pRepresentation = pRep; }
		AVOID		SetCurrentTransform(Mat4x4 const & transform, AREAL64 r64CurrentTime)	
		{
			m_currentWorldMatrix = transform; 
			if (m_pRepresentation)
				m_pRepresentation->SetCurrentTransform(m_currentWorldMatrix, r64CurrentTime);
		}

		/***** Updating *****/
		AVIRTUAL AVOID	VUpdate(AREAL64 r64Time, AREAL64 r64ElapsedTime);

	private:
		//AVIRTUAL AVOID VPreUpdate(AREAL64 r64Time, AREAL64 r64ElapsedTime);
		//AVIRTUAL AVOID VPostUpdate(AREAL64 r64Time, AREAL64 r64ElapsedTime);
	};

	//define Entity pointer
	typedef shared_ptr<Entity>	EntityPtr;

}; //Anubis