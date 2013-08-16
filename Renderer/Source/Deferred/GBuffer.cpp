//====================================================================================
//GBuffer.cpp
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

#include "Renderer_pch.h"
#include "GBuffer.h"

#include "Final/General.h"

using namespace Anubis;

/* ==
	==	Constructor ==
== */
GBufferOne::GBufferOne() : 
	/*m_pPosSRV(ShaderResourceView()),		m_pPosRTV(RenderTargetView()), 
	m_pNormalSRV(ShaderResourceView()),		m_pNormalRTV(RenderTargetView()), 
	m_pDiffuseSRV(ShaderResourceView()),	m_pDiffuseRTV(RenderTargetView()), 
	m_pSpecularSRV(ShaderResourceView()),	m_pSpecularRTV(RenderTargetView()) */
	m_SRVList(6), m_RTVList(6)
{
	//==
	//Initialize textures
	//==
	m_pPosTex		= make_shared<Texture2D>(Texture2D());
	m_pNormalTex	= make_shared<Texture2D>(Texture2D());
	m_pDepthTex		= make_shared<Texture2D>(Texture2D());
	m_pDiffuseTex	= make_shared<Texture2D>(Texture2D());
	m_pSpecularTex	= make_shared<Texture2D>(Texture2D());	
	m_pGeometryNormalTex = make_shared<Texture2D>(Texture2D());
}

ABOOL GBufferOne::VInitialize()
{
	//==
	//Define properties for textures
	//==
	Texture2DParams * pParams = new Texture2DParams();
	pParams->Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, TEX_R32G32B32A32_FLOAT, true, false, 
		true, false, 1, 0, 1, true, false, false);
		//==
	//Create textures
	//==
	Texture2D pTex = *m_pPosTex;
	pTex.Create(pParams);
	m_pPosTex->Create(pParams);
	m_pNormalTex->Create(pParams);
	m_pDepthTex->Create(pParams);
	m_pDiffuseTex->Create(pParams);
	m_pSpecularTex->Create(pParams);
	m_pGeometryNormalTex->Create(pParams);

	//Define properties for shader resource views
	ShaderResourceViewParams * pSRVParams = new ShaderResourceViewParams();
	pSRVParams->InitForTexture2D(pParams->Format, 1, 0);

	//==
	//Create Shader Resource Views
	//==
	m_pPosTex->CreateShaderResourceView(m_SRVList.GetView(0), pSRVParams);
	m_pNormalTex->CreateShaderResourceView(m_SRVList.GetView(1), pSRVParams);
	m_pDepthTex->CreateShaderResourceView(m_SRVList.GetView(2), pSRVParams);
	m_pDiffuseTex->CreateShaderResourceView(m_SRVList.GetView(3), pSRVParams);
	m_pSpecularTex->CreateShaderResourceView(m_SRVList.GetView(4), pSRVParams);
	m_pGeometryNormalTex->CreateShaderResourceView(m_SRVList.GetView(5), pSRVParams);

	//Define properties for shader resource views
	RenderTargetViewParams * pRTVParams = new RenderTargetViewParams();
	pRTVParams->InitForTexture2D(pParams->Format, 0);

	//==
	//Create Render Target Views
	//==
	m_pPosTex->CreateRenderTargetView(m_RTVList.GetView(0), pRTVParams);
	m_pNormalTex->CreateRenderTargetView(m_RTVList.GetView(1), pRTVParams);
	m_pDepthTex->CreateRenderTargetView(m_RTVList.GetView(2), pRTVParams);
	m_pDiffuseTex->CreateRenderTargetView(m_RTVList.GetView(3), pRTVParams);
	m_pSpecularTex->CreateRenderTargetView(m_RTVList.GetView(4), pRTVParams);
	m_pGeometryNormalTex->CreateRenderTargetView(m_RTVList.GetView(5), pRTVParams);

	return 1;
}

AVOID GBufferOne::BindForReading(AUINT16 slot) const
{
	m_SRVList.Set(slot, ST_Pixel); //bind g-buffer for writing
}

AVOID GBufferOne::UnbindFromReading(AUINT16 slot) const
{
	UnbindShaderResourceViews(slot, 6, ST_Pixel);
}

AVOID GBufferOne::BindForWriting(DepthStencilView * pView)
{
	m_RTVList.Clear(); //clear all render targets
	m_RTVList.Set(pView); //bind them to the pipeline
}

AVOID GBufferOne::UnbindFromWriting() const
{
	UnbindRenderTargetViews(6);
}

AVOID GBufferOne::Clear()
{
	m_RTVList.Clear();
}

AVOID GBufferOne::BindPositionTex(AUINT16 slot, ShaderType shaderType) const
{
	m_SRVList.SetView(0, slot, shaderType);
}

AVOID GBufferOne::BindNormalTex(AUINT16 slot, ShaderType shaderType) const
{
	m_SRVList.SetView(1, slot, shaderType);
}

AVOID GBufferOne::BindDepthTex(AUINT16 slot, ShaderType shaderType) const
{
	m_SRVList.SetView(2, slot, shaderType);
}

AVOID GBufferOne::BindDiffuseTex(AUINT16 slot, ShaderType shaderType) const
{
	m_SRVList.SetView(3, slot, shaderType);
}

AVOID GBufferOne::BindSpecularTex(AUINT16 slot, ShaderType shaderType) const
{
	m_SRVList.SetView(4, slot, shaderType);
}