#pragma once
//====================================================================================
//GrassPack.h
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

#include "Final\Resources\Buffers\VertexBuffer.h"
#include "..\Other\Material.h"
#include "..\Other\ShaderBunch.h"

#include "../../Math/Source/Mat4x4.h"
#include "../../Math/Source/Vectors.h"

#include "../../Renderer/Source/Renderer.h"
#include "../../Renderer/Source/Meshes/Meshes.h"

namespace Anubis
{
	class GrassPack : public Mesh
	{
	public:
		GrassPack(	const Vec & center, const AREAL r32HalfWidth, const AREAL r32HalfHeight,
					const AINT32 & num);
		~GrassPack();

		/*** Mutators ***/
		AVOID SetDiffuse(AWSTRING diffuseFileName);
		AVOID SetMask(AWSTRING maskFileName);
		AVOID SetSpecular(AWSTRING specularFileName);
		AVOID SetWorldTransform(const Mat4x4 & transform);

		/*** Getters ***/
		AINLINE Mat4x4 GetWorldTransform() const { return m_worldTransform; }

		/** Rendering methods **/
		AVIRTUAL AVOID VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection);		//change pipeline state if needed
		AVIRTUAL AVOID VRender(Renderer * pRenderer);	
		AVIRTUAL AVOID VPostRender(Renderer * pRenderer);

	protected:
		Mat4x4 m_worldTransform;
	};
}; //namespace Anubis
