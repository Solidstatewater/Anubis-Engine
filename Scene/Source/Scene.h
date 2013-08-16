#pragma once
//====================================================================================
//Scene.h
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

#include "../../Math/Source/Mat4x4.h"

#include "Renderer.h"

#include "Representations\Representation.h"
#include "Representations\Root.h"

//#include "Representations\StaticRepresentation.h"

namespace Anubis
{
	class StaticRepresentation;
	class Scene
	{
	protected:
		//store current transform
		MatrixStack	m_transforms;

		//root node
		RootRepresentation*	m_pRoot;

		typedef vector<StaticRepresentation*> StaticObjects;
		StaticObjects m_staticObjects;

		//fixed time step
		AREAL64	m_r64FixedTimeStep;

	public:
		//Constructor and Destructor
		Scene();
		~Scene();

		/***	====	Initialization	====	***/
		AVIRTUAL ABOOL VInitialize();

		/***	====	Updating	====	***/
		AVIRTUAL AVOID VUpdate(AREAL64 r64Time, AREAL64 r64ElapsedTime);

		/***	====	Rendering	====	***/
		AVIRTUAL AVOID VRender(Renderer * pRenderer, AREAL64 r64CurrentTime);

		//Representations management
		EntityRepresentation* GetRepresentation(EntityRepresentationId id);
		ABOOL RemoveRepresentation(EntityRepresentationId id);
		AVOID AddRepresentation(EntityRepresentation* pRepresentation,
			RenderPass pass = RenderPass_Actors, EntityRepresentationId parentId = 0);

		AVOID AddStaticObject(StaticRepresentation* pStatic);

		//update current world transform
		AVOID PushTransform(const Mat4x4 & mat);

		//get current tranform
		Mat4x4 GetTransform() const;

		//remove last transform
		AVOID PullTransform();

		//	=============  //
		//	Accessors
		//	=============  //
		AREAL64 GetFixedTimeStep() const { return m_r64FixedTimeStep; }
	};
};