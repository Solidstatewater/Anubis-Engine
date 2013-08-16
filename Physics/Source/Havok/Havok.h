#pragma once
//====================================================================================
//Havok.h : Havok based havok wrapper
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

#include "Entities\Entity.h"

#include "../../Math/Source/Vectors.h"
#include "../../Math/Source/Mat4x4.h"

#include "../PhysicsInterface.h"

#include <map>
namespace Anubis
{
	struct PhysicsMaterial
	{
		AREAL32	m_r32Restitution;
		AREAL32 m_r32Friction;

		PhysicsMaterial()
		{
			m_r32Restitution = 0.0f;
			m_r32Friction = 0.0f;
		}

		PhysicsMaterial(const AREAL32 r32Restitution, const AREAL32 r32Friction)
		{
			m_r32Restitution = r32Restitution;
			m_r32Friction = r32Friction;
		};

		PhysicsMaterial(const PhysicsMaterial & material)
		{
			m_r32Restitution = material.m_r32Restitution;
			m_r32Friction = material.m_r32Friction;
		};
	};

	//struct MotionState : public hkMotionState
	//{

	//class HavokPhysics;
	IPhysics* CreateHavokPhysics();
}; //Anubis