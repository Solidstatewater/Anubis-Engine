//====================================================================================
//Light.cpp
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
#include "Light.h"
#include "Final\General.h"
#include "../Renderer.h"

using namespace Anubis;

Light::~Light()
{
	SAFE_DELETE(m_pData);
	SAFE_DELETE(m_pShadowMapDSV);
	SAFE_DELETE(m_pShadowMapSRV);
	SAFE_DELETE(m_pShadowMapTex);

	SAFE_DELETE(m_pVarianceShadowRTV);
	SAFE_DELETE(m_pVarianceShadowSRV);
	SAFE_DELETE(m_pVarianceShadowUAV);
	SAFE_DELETE(m_pVarianceShadowTex);

	SAFE_DELETE(m_pTempUAV);
	SAFE_DELETE(m_pTempSRV);
	SAFE_DELETE(m_pTempTexture);
}

ABOOL Light::VInitialize(INPUT_LAYOUT * pLayout)
{
	//Initialize default light shaders
	m_pShaders = new ShaderBunch();
	m_pShaders->VSetVertexShader(m_vertexShaderFile,	DEFAULT_VERTEX_SHADER_NAME, pLayout, 2, TOPOLOGY_TRIANGLELIST);
	m_pShaders->VSetPixelShader(m_pixelShaderFile,	DEFAULT_PIXEL_SHADER_NAME);

	//Initialize textures for shadow mapping
	Texture2DParams tex2DParams;
	tex2DParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R32_TYPELESS, true, false, false, true, 1, 0,
		1, true, false, false);
	m_pShadowMapTex->Create(&tex2DParams);

	//create shader resource and render target view for the texture
	ShaderResourceViewParams srvParams;
	srvParams.InitForTexture2D(DXGI_FORMAT_R32_FLOAT, 0, 0);
	m_pShadowMapTex->CreateShaderResourceView(&m_pShadowMapSRV->m_pView, &srvParams);

	DepthStencilViewParams dsvParams;
	dsvParams.InitForTexture2D(DXGI_FORMAT_D32_FLOAT, 0);
	if (!m_pShadowMapTex->CreateDepthStencilView(&m_pShadowMapDSV->m_pView, &dsvParams))	return false;

	/*** Initialize texture for varianbce shadow mapping ***/
	tex2DParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true, true, true, false,
		1, 0, 8, true, false, false, true);
	m_pVarianceShadowTex->Create(&tex2DParams);
	m_pTempTexture->Create(&tex2DParams);

	//create shadow resource view
	srvParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 8, 0);
	m_pVarianceShadowTex->CreateShaderResourceView(&m_pVarianceShadowSRV->m_pView, &srvParams);
	m_pTempTexture->CreateShaderResourceView(&m_pTempSRV->m_pView, &srvParams);

	RenderTargetViewParams rtvParams;
	rtvParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pVarianceShadowTex->CreateRenderTargetView(&m_pVarianceShadowRTV->m_pView, &rtvParams);

	UnorderedAccessViewParams uavParams;
	uavParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	m_pVarianceShadowTex->CreateUnorderedAccessView(&m_pVarianceShadowUAV->m_pView, &uavParams);
	m_pTempTexture->CreateUnorderedAccessView(&m_pTempUAV->m_pView, &uavParams);

	m_bInitialized = true;

	return true;
}

AVOID Light::VPreRender(Renderer *pRenderer)
{
	//set shadow map
	if (pRenderer->m_bVarianceShadows)
	{
		D3D11DeviceContext()->GenerateMips(m_pVarianceShadowSRV->m_pView);
		m_pVarianceShadowSRV->Set(7, ST_Pixel);
	}
	else
	{
		m_pShadowMapSRV->Set(7, ST_Pixel);
	}

	//set shader
	m_pShaders->VBind();

	//bind constant buffer to the pipeline
	struct LightBuffer
	{
		Vec color;
		Vec pos;
		Vec dir;
		Mat4x4 view;
		Mat4x4 viewProj;
	};
	LightBuffer lightBuffer;
	lightBuffer.color = m_pData->m_color;
	lightBuffer.pos = m_pData->m_pos;
	lightBuffer.dir = m_pData->m_dir;
	lightBuffer.view = m_view;
	lightBuffer.view.Transpose();
	lightBuffer.viewProj = m_viewProjection;
	lightBuffer.viewProj.Transpose();

	pRenderer->m_pcbLight->UpdateSubresource(0, NULL, &lightBuffer, 0, 0);
	pRenderer->m_pcbLight->Set(1, ST_Pixel);

	//set sampler states
	pRenderer->AnisotropySampler16()->Set(1, ST_Pixel);
	//pRenderer->LinearTiledSampler()->Set(1, ST_Pixel);
	pRenderer->LinearLessEqualSampler()->Set(2, ST_Pixel);

	//set rasterizer state
	//pRenderer->AllEnabledBackCullingRasterizer()->Set();
}

AVOID Light::VRender()
{
	Draw(6, 0);
}

AVOID Light::VPostRender(Renderer* pRenderer)
{
	UnbindShaderResourceViews(7, 1, ST_Pixel);
	pRenderer->AllDisabledBackCullingRasterizer()->Set();
}

AVOID Light::VPrepareToGenerateShadowMap(const Mat4x4 & world, Renderer * pRenderer)
{
	//Set depth stencil view
	if (pRenderer->m_bVarianceShadows)
	{
		m_pVarianceShadowRTV->Set(*m_pShadowMapDSV);
		//m_pVarianceShadowRTV->Set();
	}
	else
	{
		SetDepthStencilView(m_pShadowMapDSV);
	}
	//GetRenderTargetView()->Set(*m_pShadowMapDSV);

	//set constant buffer with orthographic projection
	Vec dir = Vector(m_pData->m_dir.x, m_pData->m_dir.y, m_pData->m_dir.z, 0.0f);
	Mat4x4 view = CreateViewMatrixLH(m_pData->m_pos, dir, Vector(0.0f, 1.0f, 0.0f, 0.0f));
	Mat4x4 ortho = CreateOrthoProjectionLH(m_iShadowMapWidth, m_iShadowMapHeight, 0.5f, m_r32Range);
	//Mat4x4 ortho = CreateOrthoProjectionLH(m_iShadowMapWidth, m_iShadowMapHeight, 2.0f, 60.0f);
	//Mat4x4 ortho = CreatePerspectiveProjectionLH( 0.25f * 3.14f, (AREAL)SCREEN_WIDTH / (AREAL)SCREEN_HEIGHT, 0.5f, 30.0f);
	
	m_view = view;
	m_viewProjection = view * ortho;

	Mat4x4 WVP = world * m_viewProjection;
	WVP.Transpose();

	if (pRenderer->m_bVarianceShadows)
	{
		Mat4x4 WorldView = world * view;
		WorldView.Transpose();

		struct MatrixBuffer
		{
			Mat4x4 WorldView;
			Mat4x4 WVP;
		};
		MatrixBuffer buffer;
		buffer.WorldView = WorldView;
		buffer.WVP = WVP;
		pRenderer->m_pcbWorldPlusWVP->UpdateSubresource(0, NULL, &buffer, 0, 0);
		pRenderer->m_pcbWorldPlusWVP->Set(0, ST_Vertex);
	}
	else
	{
		pRenderer->m_pcbWVP->UpdateSubresource(0, NULL, &WVP, 0, 0);
		pRenderer->m_pcbWVP->Set(0, ST_Vertex);
	}

	//Actually stores length of light frustum
	Vec frustumSize = Vector(m_r32Range - 0.5, m_r32Range -0.5, m_r32Range - 0.5, m_r32Range - 0.5 );
	pRenderer->m_pcbCameraPos->UpdateSubresource(0, NULL, &frustumSize, 0, 0);
	pRenderer->m_pcbCameraPos->Set(0, ST_Pixel);
}

AVOID Light::ClearShadowMap()
{
	AREAL bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	ClearDepthStencilView(true, false, 1.0f, 0xFF, m_pShadowMapDSV);
	ClearRenderTargetView(bgColor, m_pVarianceShadowRTV);
} 

AVOID Light::VFinishShadowMapGeneration(Renderer * pRenderer)
{
	if (pRenderer->m_bVarianceShadows)
	{
		SetRenderTargetView();
	}
}