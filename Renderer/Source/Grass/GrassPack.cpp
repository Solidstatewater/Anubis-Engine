//====================================================================================
//GrassPack.cpp
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
#include "GrassPack.h"

#include "../Renderer.h"
#include "Final\General.h"
#include "Final\Resources\ResourceData.h"

#include "../../Math/Source/Math.h"

using namespace Anubis;

GrassPack::GrassPack(	const Vec & center, const AREAL r32HalfWidth, const AREAL r32HalfHeight,
						const AINT32 & num)
{
	const AREAL size_x = 0.5f;
	const AREAL size_y = 0.7f;

	m_worldTransform = Mat4x4::Identity().CreateTranslation(center);
	m_pVertices = new VertexBuffer();
	m_pTexCoords = new VertexBuffer();
	m_pNormals = new VertexBuffer();

	m_pMaterial = new Material();
	m_pShaders = new ShaderBunch();

	//Initialize vertex buffers
	BufferParams params;
	params.FillVertexBufferParams(sizeof(float3), 18 * num, true, false, false, false);

	SubresourceData data;

#pragma region Vertex buffers
	float3* pos = new float3[18 * num];
	float2* texCoords = new float2[18 * num];
	float3* normals = new float3[18 * num];
	
	//calculate grass quad offsets
	Mat4x4 rotCW = Mat4x4::Identity();
	Mat4x4 rotCCW = Mat4x4::Identity();
	rotCW = rotCW.CreateRotationY(Pi / 4.0);
	rotCCW = rotCCW.CreateRotationY(-Pi / 4.0);

	Vec leftTopVec		= Vector(-size_x, size_y, 0.0f, 0.0f);
	Vec rightTopVec		= Vector(size_x, size_y, 0.0f, 0.0f);
	Vec leftBottomVec	= Vector(-size_x, -size_y, 0.0f, 0.0f);
	Vec rightBottomVec	= Vector(size_x, -size_y, 0.0f, 0.0f);
	float3 leftTop1		= leftTopVec;
	float3 rightTop1	= rightTopVec;
	float3 leftBottom1	= leftBottomVec;
	float3 rightBottom1	= rightBottomVec;
	float3 leftTop2		= leftTopVec * rotCW;
	float3 rightTop2	= rightTopVec * rotCW;
	float3 leftBottom2	= leftBottomVec * rotCW;
	float3 rightBottom2 = rightBottomVec * rotCW;
	float3 leftTop3		= leftTopVec * rotCCW;
	float3 rightTop3	= rightTopVec * rotCCW;
	float3 leftBottom3	= leftBottomVec * rotCCW;
	float3 rightBottom3 = rightBottomVec * rotCCW;

	float3 normal = leftTop1 - leftBottom1;

	AINT32 numWidth = num / (AREAL)(r32HalfHeight + r32HalfHeight);
	AREAL stepWidth  =  (r32HalfWidth * 2) / (AREAL)numWidth;

	AINT32 numHeight = num / numWidth;
	AREAL stepHeight = (r32HalfHeight * 2) / (AREAL)numHeight;

	for (AINT32 i = 0; i < num; i++)
	{
		///////////////////////////////////////////////////
		//Calculate block center position
		//AINT32 x = Random(-r32HalfWidth, r32HalfWidth);
		//AINT32 y = Random(-r32HalfHeight, r32HalfHeight);
		AREAL x = (i % numWidth) * stepWidth;
		AREAL z = (i / numWidth) * stepHeight;

		float3 blockCenter = float3(getx(center) + x, gety(center), getz(center) + z);

		///////////////////////////////////////////////////
		//Calculate invidual vertex data for each block

		//first block
		pos[i*18]		= blockCenter + leftTop1;
		pos[i*18+1]		= blockCenter + rightTop1;
		pos[i*18+2]		= blockCenter + leftBottom1;
		pos[i*18+3]		= blockCenter + leftBottom1;
		pos[i*18+4]		= blockCenter + rightTop1;
		pos[i*18+5]		= blockCenter + rightBottom1;

		texCoords[i*18]	  = float2(0.0f, 0.0f);
		texCoords[i*18+1] = float2(1.0f, 0.0f);
		texCoords[i*18+2] = float2(0.0f, 1.0f);
		texCoords[i*18+3] = float2(0.0f, 1.0f);
		texCoords[i*18+4] = float2(1.0f, 0.0f);
		texCoords[i*18+5] = float2(1.0f, 1.0f);

		normals[i*18]	= normal;
		normals[i*18+1] = normal;
		normals[i*18+2] = normal;
		normals[i*18+3] = normal;
		normals[i*18+4] = normal;
		normals[i*18+5] = normal;

		//second block
		pos[i*18+6]		= blockCenter + leftTop2;
		pos[i*18+7]		= blockCenter + rightTop2;
		pos[i*18+8]		= blockCenter + leftBottom2;
		pos[i*18+9]		= blockCenter + leftBottom2;
		pos[i*18+10]	= blockCenter + rightTop2;
		pos[i*18+11]	= blockCenter + rightBottom2;

		texCoords[i*18+6]	  = float2(0.0f, 0.0f);
		texCoords[i*18+7] = float2(1.0f, 0.0f);
		texCoords[i*18+8] = float2(0.0f, 1.0f);
		texCoords[i*18+9] = float2(0.0f, 1.0f);
		texCoords[i*18+10] = float2(1.0f, 0.0f);
		texCoords[i*18+11] = float2(1.0f, 1.0f);

		normals[i*18+6]	= normal;
		normals[i*18+7] = normal;
		normals[i*18+8] = normal;
		normals[i*18+9] = normal;
		normals[i*18+10] = normal;
		normals[i*18+11] = normal;

		//third block
		pos[i*18+12]	= blockCenter + leftTop3;
		pos[i*18+13]	= blockCenter + rightTop3;
		pos[i*18+14]	= blockCenter + leftBottom3;
		pos[i*18+15]	= blockCenter + leftBottom3;
		pos[i*18+16]	= blockCenter + rightTop3;
		pos[i*18+17]	= blockCenter + rightBottom3;

		texCoords[i*18+12] = float2(0.0f, 0.0f);
		texCoords[i*18+13] = float2(1.0f, 0.0f);
		texCoords[i*18+14] = float2(0.0f, 1.0f);
		texCoords[i*18+15] = float2(0.0f, 1.0f);
		texCoords[i*18+16] = float2(1.0f, 0.0f);
		texCoords[i*18+17] = float2(1.0f, 1.0f);

		normals[i*18+12]	= normal;
		normals[i*18+13] = normal;
		normals[i*18+14] = normal;
		normals[i*18+15] = normal;
		normals[i*18+16] = normal;
		normals[i*18+17] = normal;
	}

	params.SetSize(sizeof(float3)*18*num);
	data.SetData(pos);
	if (!m_pVertices->Create(&params, &data, 18*num, sizeof(float3)))
	{
		assert(0 && "Error creating vertex buffer in Grass initialization!");
	}

	params.SetSize(sizeof(float2)*18*num);
	data.SetData(texCoords);
	if (!m_pTexCoords->Create(&params, &data, 18*num, sizeof(float2)))
	{
		assert(0 && "Error creating vertex buffer in Grass initialization!");
	}

	params.SetSize(sizeof(float3)*18*num);
	data.SetData(normals);
	if (!m_pNormals->Create(&params, &data, 18*num, sizeof(float3)))
	{
		assert(0 && "Error creating vertex buffer in Grass Initialization!");
	}
#pragma endregion

	//Initilize input layout
	INPUT_LAYOUT layout[] =
	{
		{ "POSITION", 0, TEX_R32G32B32_FLOAT, 0, 0, IA_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, TEX_R32G32_FLOAT,	  1, 0, IA_PER_VERTEX_DATA, 0},
		{ "NORMAL",   0, TEX_R32G32B32_FLOAT, 2, 0, IA_PER_VERTEX_DATA, 0},
	};

	m_pShaders->VSetVertexShader(L"Grass_Vertex.hlsl", "Grass_VS", layout, 3, TOPOLOGY_TRIANGLELIST);
	m_pShaders->VSetPixelShader(L"Grass_Pixel.hlsl", "Grass_PS");
}

GrassPack::~GrassPack()
{
	SAFE_DELETE(m_pNormals);
	SAFE_DELETE(m_pTexCoords);
	SAFE_DELETE(m_pVertices);
}

AVOID GrassPack::SetWorldTransform(const Mat4x4 & transform)
{
	m_worldTransform = transform;
}

AVOID GrassPack::VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection)
{
	//update constant buffers
	struct MatrixBuffer
	{
		Mat4x4 m_world;
		Mat4x4 m_WVP;
	};
	MatrixBuffer buffer;
	buffer.m_world = m_worldTransform;
	buffer.m_WVP = m_worldTransform * viewprojection;
	buffer.m_world.Transpose();
	buffer.m_WVP.Transpose();

	pRenderer->m_pcbWorldPlusWVP->UpdateSubresource(0, NULL, &buffer, 0, 0);
	pRenderer->m_pcbWorldPlusWVP->Set(0, ST_Vertex);

	//set vertex buffer with positions
	m_pVertices->Set(0, 0);

	//set vertex buffer with texture data
	if (pRenderer->TexturingOn())
		m_pTexCoords->Set(1, 0);

	//set vertex buffer with normal data
	if (pRenderer->LightningOn())
		m_pNormals->Set(2, 0);

	//set material
	//m_pMaterial->Set(0);
	m_pMaterial->BindDiffuse(0);
	m_pMaterial->BindHeight(1);
	m_pMaterial->BindSpecular(2);

	//set shaders
	m_pShaders->VBind();

	pRenderer->AnisotropySampler16()->Set(0, ST_Pixel);

	//set rasterizer state
	pRenderer->NoCullingStandardRasterizer()->Set();

	//set blend state
	pRenderer->BlendAddStandard()->Set(NULL);
}

AVOID GrassPack::SetDiffuse(AWSTRING diffuseFileName)
{
	m_pMaterial->SetDiffuseTexture(diffuseFileName);
}

AVOID GrassPack::SetMask(AWSTRING maskFileName)
{
	m_pMaterial->SetHeightTexture(maskFileName);
}

AVOID GrassPack::SetSpecular(AWSTRING specularFileName)
{
	m_pMaterial->SetSpecularTexture(specularFileName);
}

AVOID GrassPack::VRender(Renderer * pRenderer)
{
	Draw(m_pVertices->Count(), 0);
}

AVOID GrassPack::VPostRender(Renderer * pRenderer)
{
	m_pMaterial->UnbindDiffuse(0);
	m_pMaterial->UnbindHeight(0);

	//return blend state
	pRenderer->NoBlending()->Set(NULL);
}