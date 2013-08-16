#pragma once
//====================================================================================
//PhysicsInterface.h : common methods
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

namespace Anubis
{
	//******************************************
	// Represents Interface to hold game physics
	//******************************************
	class IPhysics
	{
	public:

		/////////////////////////
		/*** General Methods ***/
		/////////////////////////

		//Initialize physics
		AVIRTUAL ABOOL VInit() = 0;
		
		//sychronize with engine
		AVIRTUAL AVOID VSyncData(AREAL64 r64CurrentTime) = 0;

		//Update simulation
		AVIRTUAL AVOID VUpdate(const AREAL deltaSeconds ) = 0;

		///////////////////////////////////////
		/*** Add objects to the simulation ***/
		///////////////////////////////////////
		AVIRTUAL AVOID VAddBox(EntityPtr pEntity, Vec & pos, Vec & dimensions, AREAL r32Mass, ASTRING & physMat, ABOOL isStatic) = 0;
		AVIRTUAL AVOID VAddPlane(EntityPtr pEntity, Vec & pos, Vec & halfDimensions, AREAL r32Mass, ASTRING & physMat, ABOOL isStatic) = 0;
		//AVIRTUAL AVOID VAddSphere(EntityPtr pEntity, const AREAL radius, 

		/////////////////////////////////////
		/*** Apply actions to the bodies ***/
		/////////////////////////////////////

		AVIRTUAL AVOID VApplyForce(const EntityId entityId, const AREAL r32DeltaTime, const Vec & dir, const AREAL newtons) = 0;
		AVIRTUAL AVOID VApplyTorque(const EntityId entityId, const AREAL r32DeltaTime, const Vec & dir, const AREAL newtons) = 0;
		AVIRTUAL AVOID VKinematicMovement(const EntityId entityId, const Mat4x4 & mat) = 0;

		/////////////////////////////////////
		//getters
		AVIRTUAL Vec VGetVelocity(const EntityId entityId) = 0;
		AVIRTUAL Vec VGetAngularVelocity(const EntityId entityId) = 0;
		AVIRTUAL Mat4x4 VGetTransform(const EntityId entityId) = 0;

		/////////////////////////////////////
		//setters
		AVIRTUAL AVOID VSetVelocity(const EntityId entityId, const Vec & vel) = 0;
		AVIRTUAL AVOID VSetAngularVelocity(const EntityId entityId, const Vec & angularVel) = 0;
		//AVIRTUAL AVOID VSetTransform(const EntityId entityId, const Mat4x4 & mat);
	};
};