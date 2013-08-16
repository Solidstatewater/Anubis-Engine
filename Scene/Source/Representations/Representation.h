#pragma once
//====================================================================================
//Representation.h
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

#include "../../../Math/Source/Vectors.h"
#include "../../../Math/Source/Mat4x4.h"
#include "Renderer.h"

namespace Anubis
{
	//Unique identifier for each representation
	typedef AUINT32	EntityRepresentationId;

	//define pointer to the representation
	class EntityRepresentation;
	typedef shared_ptr<EntityRepresentation> EntityRepresentationPtr;

	class Scene;
	class EntityRepresentation
	{
	protected:

		Mat4x4 m_currentWorldMatrix;	//matrix in parent space (world matrix for the 1st layer)

		//unique id
		EntityRepresentationId	m_id;

		//time of last update
		AREAL64	m_r64LastUpdate;

		//typedef shared_ptr<EntityRepresentation> EntityRepresentationPtr;
		typedef vector<EntityRepresentation*> Representations;

		//link to the children
		Representations	m_children;

		// ======= Visualization ======= //
		Meshes	m_meshes;
		Lights	m_lights;

		// ======= Visibility Determination ======= //
		float3	m_f3Size;

	public:
		//Constructor
		EntityRepresentation(EntityRepresentationId id) : m_id(id), m_r64LastUpdate(0.0) { 
			m_f3Size = float3(0.0f, 0.0f, 0.0f); 
			m_currentWorldMatrix = Mat4x4::Identity(); 
		}
		EntityRepresentation(EntityRepresentationId id, const float3 & f3Size )
			: m_id(id), m_r64LastUpdate(0.0) {
				m_f3Size = f3Size;
				m_currentWorldMatrix = Mat4x4::Identity();
		}
		EntityRepresentation(EntityRepresentationId id, const ASTRING resourceMesh);
		~EntityRepresentation();

		//Add and remove children
		AVIRTUAL AVOID VAddChild(EntityRepresentation* pRepr);
		AVIRTUAL AVOID VRemoveChild(EntityRepresentationId id);

		//Add meshes and lights
		AVIRTUAL AVOID VAddMesh(MeshPtr pMesh);
		AVIRTUAL AVOID VAddLight(LightPtr pLight);

		//find child
		AVIRTUAL EntityRepresentation* VFindChild(EntityRepresentationId  id);

		//put all low-level mesh data to the renderer
		AVIRTUAL AVOID VRender(Scene * pScene, Renderer * pRenderer, AREAL64 r64CurrentTime);

		//	=================  //
		//		Accessors	   //
		//	=================  //
		EntityRepresentationId	GetId() const { return m_id; }

		//	================  //
		//		Mutators	  //
		//	================  //
		AVIRTUAL AVOID SetCurrentTransform(Mat4x4 & mat, AREAL64 r64CurrentTime);
		AVOID SetSize(const AREAL sizex, const AREAL sizey, const AREAL sizez)
		{
			m_f3Size.x = sizex;
			m_f3Size.y = sizey;
			m_f3Size.z = sizez;
		}

	protected:

		//pre-rendering modifications
		AVIRTUAL AVOID VPushParameters(Scene *pScene, AREAL64 r64CurrentTime);

		//push data to the renderer
		AVIRTUAL AVOID VSendRenderingData(Scene * pScene, Renderer * pRenderer);

		//undo any changes made to the scene
		AVIRTUAL AVOID VPullParameters(Scene * pScene);

	};

}; //Anubis