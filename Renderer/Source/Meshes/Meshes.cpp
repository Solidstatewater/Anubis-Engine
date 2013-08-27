//====================================================================================
//Meshes.cpp
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
#include "Meshes.h"

#include "../Renderer.h"
#include "Final\General.h"

using namespace Anubis;

Mesh::Mesh()
{
	m_objectTransform = Mat4x4::Identity();
	m_worldTransform = Mat4x4::Identity();

	m_pVertices = NULL;
	m_pTexCoords = NULL;
	m_pNormals = NULL;

	m_pMaterial = new Material();

	m_pShaders = NULL;
}

Mesh::~Mesh()
{
	SAFE_DELETE(m_pNormals);
	SAFE_DELETE(m_pTexCoords);
	SAFE_DELETE(m_pVertices);
}

AVOID Mesh::VSetMaterial(	AWSTRING diffuseFileName, AWSTRING normalFileName,
							AWSTRING specularFileName,AWSTRING heightFileName)
{
	m_pMaterial->VInitialize(diffuseFileName, normalFileName, specularFileName, heightFileName);
}

AVOID Mesh::SetWorldTransform(const Mat4x4 & transform)
{
	m_worldTransform = m_objectTransform * transform;
}

//AVOID Mesh::VSetVertexShader	(AWSTRING fileName, ASTRING shaderName);
//AVOID Mesh::VSetHullShader		(AWSTRING fileName, ASTRING shaderName);
//AVOID Mesh::VSetDomainShader	(AWSTRING fileName, ASTRING shaderName);
//AVOID Mesh::VSetGeometryShader	(AWSTRING fileName, ASTRING shaderName);
//AVOID Mesh::VSetPixelShader		(AWSTRING fileName, ASTRING shaderName);

AVOID Mesh::GenerateShadowMap()
{
	m_pVertices->Set(0, 0);
	Draw(m_pVertices->Count(), 0);
}

AVOID Mesh::VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection)
{
	//update constant buffers
	struct MatrixBuffer
	{
		Mat4x4 m_world;
		Mat4x4 m_worldView;
		Mat4x4 m_WVP;
	};
	MatrixBuffer buffer;
	buffer.m_world = m_worldTransform;
	buffer.m_worldView = m_worldTransform * view;
	buffer.m_WVP = m_worldTransform * viewprojection;
	buffer.m_world.Transpose();
	buffer.m_worldView.Transpose();
	buffer.m_WVP.Transpose();
	pRenderer->m_pcbWorldPlusWorldViewPlusWVP->UpdateSubresource(0, NULL, &buffer, 0, 0);
	pRenderer->m_pcbWorldPlusWorldViewPlusWVP->Set(0, ST_Vertex);

	//set vertex buffer with positions
	m_pVertices->Set(0, 0);

	//set vertex buffer with texture data
	if (pRenderer->TexturingOn())
		m_pTexCoords->Set(1, 0);

	//set vertex buffer with normal data
	if (pRenderer->LightningOn())
		m_pNormals->Set(2, 0);

	//set material
	m_pMaterial->Set(0);

	//set shaders
	if (m_pShaders)
		m_pShaders->VBind();
}

AVOID Mesh::VRender(Renderer * pRenderer)
{
	Draw(m_pVertices->Count(), 0);
}

AVOID Mesh::VPostRender(Renderer * pRenderer)
{
	m_pMaterial->Unbind();
}

//////////////////////////////////////////////
//Indexed mesh
IndexedMesh::~IndexedMesh()
{
	SAFE_DELETE(m_pIndexBuffer);
}

AVOID IndexedMesh::VPreRender(Renderer *pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection)
{
	Mesh::VPreRender(pRenderer, view, viewprojection);

	//set index buffer
	m_pIndexBuffer->Set(0);
}

AVOID IndexedMesh::VRender(Renderer* pRenderer)
{
	DrawIndexed(m_pIndexBuffer->Count(), 0, 0);
}

AVOID IndexedMesh::GenerateShadowMap()
{
	m_pVertices->Set(0, 0);
	m_pIndexBuffer->Set(0);

	DrawIndexed(m_pIndexBuffer->Count(), 0, 0);
}