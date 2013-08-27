#pragma once
//====================================================================================
//Meshes.h
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

#include "Final/Resources/Buffers/Buffers.h"
#include "../Other/Material.h"
#include "../Other/ShaderBunch.h"
#include "../../Math/Source/Mat4x4.h"

namespace Anubis
{
	struct NormalData
	{
		float3 normal;
		float3 tangent;
		float3 binormal;
	};

	class Renderer;
	class Mesh 
	{
	private:
		Mat4x4			m_objectTransform;
		Mat4x4			m_worldTransform;

	protected:
		//Vertex buffers
		VertexBuffer*	m_pVertices;
		VertexBuffer*	m_pTexCoords;
		VertexBuffer*	m_pNormals;

		Material*		m_pMaterial;

		ShaderBunch*	m_pShaders;

	public:
		//Constructor and destructor
		Mesh();
		~Mesh();

		/**	Initialization **/
		//AVIRTUAL ABOOL VInitialize(ASTRING fileName) = 0;
		AVIRTUAL AVOID VSetMaterial(AWSTRING diffuseFileName, AWSTRING normalFileName,
									AWSTRING specularFileName,AWSTRING heightFileName);
		AVOID SetWorldTransform(const Mat4x4 & transform);
		AINLINE Mat4x4 GetWorldTransform() const { return m_worldTransform; }

		//Shaders
		//AVIRTUAL AVOID VSetVertexShader		(AWSTRING fileName, ASTRING shaderName);
		//AVIRTUAL AVOID VSetHullShader		(AWSTRING fileName, ASTRING shaderName);
		//AVIRTUAL AVOID VSetDomainShader		(AWSTRING fileName, ASTRING shaderName);
		//AVIRTUAL AVOID VSetGeometryShader	(AWSTRING fileName, ASTRING shaderName);
		//AVIRTUAL AVOID VSetPixelShader		(AWSTRING fileName, ASTRING shaderName);

		/** Rendering methods **/
		AVIRTUAL AVOID VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection);		//change pipeline state if needed
		AVIRTUAL AVOID VRender(Renderer * pRenderer);		//render the mesh
		AVIRTUAL AVOID VPostRender(Renderer * pRenderer);			//restore previous pipeline state if needed
	
		AVOID GenerateShadowMap();
	};

	//useful typedefs
	typedef shared_ptr<Mesh> MeshPtr;
	typedef vector<MeshPtr> Meshes;

	/*struct Subset
	{
		~Subset()
		{
			SAFE_DELETE(m_pVerticesBuffer);
			SAFE_DELETE(m_pTexCoordBuffer);
			SAFE_DELETE(m_pNormalDataBuffer);
			SAFE_DELETE(m_pIndexBuffer);
		}

		VertexBuffer*	m_pVerticesBuffer;
		VertexBuffer*	m_pTexCoordBuffer;
		VertexBuffer*	m_pNormalDataBuffer;
		IndexBuffer*	m_pIndexBuffer;

		std::shared_ptr<Material>	m_pMaterial;
	}; */
	/*class ComplexMesh : public Mesh
	{
	protected:
		Subset*	m_pSubsets;
		AUINT32 m_uNumSubsets;

	public:

		ComplexMesh();
		~ComplexMesh();

	
		AVOID VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection);		//change pipeline state if needed
		AVOID VRender(Renderer * pRenderer);		//render the mesh
		AVOID VPostRender(Renderer * pRenderer);			//restore previous pipeline state if needed
	
		AVOID GenerateShadowMap();
	}; */

	class IndexedMesh : public Mesh
	{
		friend class ObjResourceLoader;

	protected:
		IndexBuffer*	m_pIndexBuffer;

	public:
		IndexedMesh() : Mesh()
		{
			m_pIndexBuffer = nullptr;

			//initialize input layout 
			INPUT_LAYOUT layout[] =
			{
				{ "POSITION", 0, TEX_R32G32B32_FLOAT, 0, 0,  IA_PER_VERTEX_DATA, 0},
				{ "TEXCOORDS", 0, TEX_R32G32_FLOAT,	  1, 0,  IA_PER_VERTEX_DATA, 0},
				{ "NORMAL",   0, TEX_R32G32B32_FLOAT, 2, 0,  IA_PER_VERTEX_DATA, 0},
				{ "TANGENT", 0, TEX_R32G32B32_FLOAT, 2, 12, IA_PER_VERTEX_DATA, 0},
				{ "BINORMAL",  0, TEX_R32G32B32_FLOAT, 2, 24, IA_PER_VERTEX_DATA, 0},
			};

			//Initialize shaders
			m_pShaders = new ShaderBunch();
			m_pShaders->VSetVertexShader(L"GBufferShader.hlsl", "VS", layout, 5, TOPOLOGY_TRIANGLELIST);
			m_pShaders->VSetPixelShader(L"GBufferShader.hlsl", "PS");
		}
		~IndexedMesh();

		/** Rendering methods **/
		AVOID VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection);		//change pipeline state if needed
		AVOID VRender(Renderer * pRenderer);		//render the mesh
	
		AVOID GenerateShadowMap();
	};

}; //Anubis