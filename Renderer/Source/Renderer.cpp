//====================================================================================
//Renderer.cpp
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
#include "Renderer.h"

#include "Final/General.h"
#include "Final/Resources/Buffers/BufferParams.h"

using namespace Anubis;

Renderer::Renderer() : m_bTexturingOn(true), m_bLightningOn(true)
{
	m_pAnisotropySampler16 = NULL;
	m_pLinearTiledSampler = NULL;
	m_pPointClampSampler = NULL;
	m_pLinearLessEqualSampler = NULL;

	m_pNoCullingStandard = NULL;
	m_pAllEnabledBackCulling = NULL;
	m_pAllDisabledBackCulling = NULL;

	m_pNoBlending = NULL;
	m_pBlendAddStandard = NULL;
	m_pBlendLightPass = NULL;

	m_pDepthDisableStencilDisable = new DepthStencilState();
	m_pDepthEnableStencilDisableStandard = new DepthStencilState();

	m_pDepthTexture = new Texture2D();

	m_pDepthSRV = new ShaderResourceView();
	m_pDepthDSV = new DepthStencilView();

	m_pGaussianFilter = new ComputeShader();
	m_pGaussianHorFilter = new ComputeShader();
	m_pGaussianVerFilter = new ComputeShader();

	m_pCopyResourceShader = new ComputeShader();

	//help texture
	m_pTempTexture = new Texture2D();
	m_pTempSRV = new ShaderResourceView();
	m_pTempRTV = new RenderTargetView();
	m_pTempUAV = new UnorderedAccessView();

	//velocity buffer
	m_pVelocityTexture = new Texture2D();
	m_pVelocitySRV = new ShaderResourceView();
	m_pVelocityRTV = new RenderTargetView();
}

ABOOL Renderer::VInitialize(HWND hWnd, AUINT32 width, AUINT32 height)
{
	InitializeGraphics(hWnd, width, height);

	/*	===============================
		==	Create Shaders  ==
		===============================  */
	BlobDX11* pErrors = new BlobDX11();
	m_pGaussianFilter->CreateAndCompile(L"Gauss.hlsl", "CS", pErrors);
	m_pGaussianHorFilter->CreateAndCompile(L"Gauss_Separable.hlsl", "CS_Horizontal", pErrors);
	m_pGaussianVerFilter->CreateAndCompile(L"Gauss_Separable.hlsl", "CS_Vertical", pErrors);

	m_pCopyResourceShader->CreateAndCompile(L"Resources\\Shaders\\Other\\CopyResource.hlsl", "Copy_CS");

	//Initialize structure to fill buffer properties
	BufferParams * params = new BufferParams();

	/*	===============================
		==	Create Constant Buffers  ==
		===============================  */

	//Create View Matrix Buffer
	params->FillConstantBufferParams(sizeof(Mat4x4), true, false, false);
	m_pcbView = new ConstantBuffer();
	if (!m_pcbView->Create(params, NULL))	return false;

	m_pcbPrevViewProj = new ConstantBuffer();
	if (!m_pcbPrevViewProj->Create(params, NULL)) return false;

	//Create World + WVP Matrix Buffer
	params->FillConstantBufferParams(2*sizeof(Mat4x4), true, false, false);
	m_pcbWorldPlusWVP = new ConstantBuffer();
	if (!m_pcbWorldPlusWVP->Create(params, NULL))	return false;

	//Create World + WorldView + WVP Matrix Buffer
	params->FillConstantBufferParams(3*sizeof(Mat4x4), true, false, false);
	m_pcbWorldPlusWorldViewPlusWVP = new ConstantBuffer();
	if (!m_pcbWorldPlusWorldViewPlusWVP->Create(params, NULL))	return false;

	//Create WVP Matrix Buffer
	params->SetSize(sizeof(Mat4x4));
	m_pcbWVP = new ConstantBuffer();
	if (!m_pcbWVP->Create(params, NULL))			return false;

	//Create Position Camera Buffer
	params->SetSize(sizeof(Vec));
	m_pcbCameraPos = new ConstantBuffer();
	if (!m_pcbCameraPos->Create(params, NULL))		return false;

	//Create Light Buffer
	params->SetSize(3*sizeof(Vec)  + 2*sizeof(Mat4x4));
	m_pcbLight = new ConstantBuffer();
	if (!m_pcbLight->Create(params, NULL))			return false;

	//Create Point Light Buffer
	params->SetSize(sizeof(Vec));
	m_pcbPointLight = new ConstantBuffer();
	if (!m_pcbPointLight->Create(params, NULL))		return false;

	//Create Spot Light Buffer
	params->SetSize(sizeof(Vec));
	m_pcbSpotLight = new ConstantBuffer();
	if (!m_pcbSpotLight->Create(params, NULL))		return false;

	//Create frustum data buffer
	params->SetSize(sizeof(Vec));
	m_pcbFrustumSize = new ConstantBuffer();
	if (!m_pcbFrustumSize->Create(params, NULL))	return false;

	//Create depth buffer data buffer
	params->SetSize(sizeof(Vec));
	m_pcbDepthBuffer = new ConstantBuffer();
	if (!m_pcbDepthBuffer->Create(params, NULL))	return false;

	//Create SSAO params buffer
	params->SetSize(sizeof(Vec));
	m_pcbSSAOParams = new ConstantBuffer();
	if (!m_pcbSSAOParams->Create(params, NULL))		return false;

	params->SetSize(sizeof(Vec) + sizeof(Mat4x4));
	m_pcbCameraPosPlusInverseWP = new ConstantBuffer();
	if (!m_pcbCameraPosPlusInverseWP->Create(params, NULL))	return false;

	//Create SSAO sample points buffer
	//params->SetSize(sizeof(float3) * 8);
	//m_pcbSSAOSamples = new ConstantBuffer();
	//if (!m_pcbSSAOSamples->Create(params, NULL))	return false;

	//delete params structre
	delete params;

	/*	===================================
		==	Create Depth Stencil States  ==
		=================================== */
	DepthStencilParams depthStencilParams;
	depthStencilParams.Init(true, D3D11_COMPARISON_LESS, false, 0, 0);
	if (!m_pDepthEnableStencilDisableStandard->Create(&depthStencilParams))	return false;

	depthStencilParams.Init(false, D3D11_COMPARISON_LESS, false, 0, 0);
	if (!m_pDepthDisableStencilDisable->Create(&depthStencilParams))		return false;

	Texture2DParams dParams;
	dParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R32_TYPELESS, true, false, false, true, 1, 0,
				1, true, false, false);
	if (!m_pDepthTexture->Create(&dParams))	return false;

	ShaderResourceViewParams srvParams;
	srvParams.InitForTexture2D(DXGI_FORMAT_R32_FLOAT, 1, 0);
	if (!m_pDepthTexture->CreateShaderResourceView(&m_pDepthSRV->m_pView, &srvParams))	return false;

	DepthStencilViewParams dsvParams;
	dsvParams.InitForTexture2D(DXGI_FORMAT_D32_FLOAT, 0);
	if (!m_pDepthTexture->CreateDepthStencilView(&m_pDepthDSV->m_pView, &dsvParams))	return false;

	////////////////////////////////////////////////////
	//Velocity buffering
	Texture2DParams velTexParams;
	velTexParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true, false, true, false, 1, 0, 1, true, false, false);
	if (!m_pVelocityTexture->Create(&velTexParams)) return false;

	ShaderResourceViewParams velSRVParams;
	velSRVParams.InitForTexture2D(velTexParams.Format, 1, 0);
	if (!m_pVelocityTexture->CreateShaderResourceView(&m_pVelocitySRV->m_pView, &velSRVParams)) return false;

	RenderTargetViewParams velRTVParams;
	velRTVParams.InitForTexture2D(velTexParams.Format, 0);
	if (!m_pVelocityTexture->CreateRenderTargetView(&m_pVelocityRTV->m_pView, &velRTVParams)) return false;

	///////////////////////////////////////////
	//Create Linear depth buffer views and texture
	/*dParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true, false, true, false, 1, 0,
		1, true, false, false);
	if (!m_pLinearDepthTexture->Create(&dParams)) return false;

	srvParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0);
	if (!m_pLinearDepthTexture->CreateShaderResourceView(&m_pLinearDepthSRV->m_pView, &srvParams))	return false;

	RenderTargetViewParams rtvParams;
	rtvParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	if (!m_pLinearDepthTexture->CreateRenderTargetView(&m_pLinearDepthRTV->m_pView, &rtvParams)) return false; */

	Texture2DParams tempTexParams;
	tempTexParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, true, true, true, false, 1, 0, 1, true, false, false);
	if (!m_pTempTexture->Create(&tempTexParams)) return false;

	srvParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0);
	if (!m_pTempTexture->CreateShaderResourceView(&m_pTempSRV->m_pView, &srvParams)) return false;

	RenderTargetViewParams rtvParams;
	rtvParams.InitForTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	if (!m_pTempTexture->CreateRenderTargetView(&m_pTempRTV->m_pView, &rtvParams)) return false;

	return true;
}

Renderer::~Renderer()
{
	//Clean Consant Buffers in reverse order
	SAFE_DELETE(m_pcbSpotLight);
	SAFE_DELETE(m_pcbPointLight);
	SAFE_DELETE(m_pcbLight);
	SAFE_DELETE(m_pcbCameraPos);
	SAFE_DELETE(m_pcbWVP);
	SAFE_DELETE(m_pcbWorldPlusWorldViewPlusWVP);
	SAFE_DELETE(m_pcbWorldPlusWVP);
	SAFE_DELETE(m_pcbView);

	SAFE_DELETE(m_pcbCameraPos);
	SAFE_DELETE(m_pcbCameraPosPlusInverseWP);

	//delete sampler states
	SAFE_DELETE(m_pLinearTiledSampler);
	SAFE_DELETE(m_pAnisotropySampler16);
	SAFE_DELETE(m_pPointClampSampler);
	SAFE_DELETE(m_pLinearLessEqualSampler);

	//delete rasterizer states
	SAFE_DELETE(m_pNoCullingStandard);
	SAFE_DELETE(m_pAllEnabledBackCulling);
	SAFE_DELETE(m_pAllDisabledBackCulling);

	//delete blend states
	SAFE_DELETE(m_pNoBlending);
	SAFE_DELETE(m_pBlendAddStandard);
	SAFE_DELETE(m_pBlendLightPass);

	//clean shaders
	SAFE_DELETE(m_pGaussianFilter);
	SAFE_DELETE(m_pGaussianHorFilter);
	SAFE_DELETE(m_pGaussianVerFilter);

	SAFE_DELETE(m_pCopyResourceShader);

	//clea help texture and its views
	SAFE_DELETE(m_pTempRTV);
	SAFE_DELETE(m_pTempSRV);
	SAFE_DELETE(m_pTempUAV);
	SAFE_DELETE(m_pTempTexture);

	//clean velocity buffering
	SAFE_DELETE(m_pVelocityRTV);
	SAFE_DELETE(m_pVelocitySRV);
	SAFE_DELETE(m_pVelocityTexture);

	//remove all cameras
	while (!m_cameras.empty())
		m_cameras.pop_back();
}

AVOID Renderer::FilterImage(ShaderResourceView* pSRV, UnorderedAccessView* pResultUAV, ShaderResourceView* pTempSRV, UnorderedAccessView* pTempUAV,
							AREAL texWidth, AREAL texHeight, FilterType type)
{
	switch (type)
	{
	case FT_Gaussian:
		m_pGaussianFilter->Set();
		pSRV->Set(0, ST_Compute);
		pTempUAV->Set(0, nullptr, ST_Compute);

		D3D11DeviceContext()->Dispatch(ceil(SCREEN_WIDTH / 32.0), ceil(SCREEN_HEIGHT / 32.0), 1);
		UnbindShaderResourceViews(0, 1, ST_Compute);
		UnbindUnorderedAccessViews(0, 1);

		//copy resource back to its texture
		m_pCopyResourceShader->Set();
		pTempSRV->Set(0, ST_Compute);
		pResultUAV->Set(0, nullptr, ST_Compute);

		D3D11DeviceContext()->Dispatch(ceil(SCREEN_WIDTH / 32.0), ceil(SCREEN_HEIGHT / 32.0), 1);
		UnbindShaderResourceViews(0, 1, ST_Compute);
		UnbindUnorderedAccessViews(0, 1);

		break;
	case FT_GaussianSeparable:
		m_pGaussianHorFilter->Set();
		pSRV->Set(0, ST_Compute);
		pTempUAV->Set(0, nullptr, ST_Compute);

		//horizontal pass
		D3D11DeviceContext()->Dispatch(1, 720, 1);
		UnbindShaderResourceViews(0, 1, ST_Compute);
		UnbindUnorderedAccessViews(0, 1);

		//vertical pass
		m_pGaussianVerFilter->Set();
		pTempSRV->Set(0, ST_Compute);
		pResultUAV->Set(0, nullptr, ST_Compute);

		D3D11DeviceContext()->Dispatch(1280, 1, 1);
		UnbindShaderResourceViews(0, 1, ST_Compute);
		UnbindUnorderedAccessViews(0, 1);

		break;
	};
};

AVOID Renderer::AddCamera(CameraPtr pCamera)
{
	m_cameras.push_back(pCamera);
}

AVOID Renderer::RemoveCamera(CameraPtr pCamera)
{
	//find camera in the list
	auto camera = m_cameras.begin();
	for (; camera != m_cameras.end(); camera++)
		if ((*camera) == pCamera)
			break;

	//delete camera
	if (camera != m_cameras.end())
		m_cameras.erase(camera);

	//no such camera
	ASSERT(0 && "No such camera was found!");
}

SamplerState* Renderer::AnisotropySampler16()
{
	if (m_pAnisotropySampler16)	return m_pAnisotropySampler16;

	//it's not created yet
	m_pAnisotropySampler16 = new SamplerState();

	SamplerStateParams params;
	params.Init(FILTER_ANISOTROPIC, TEXTURE_ADDRESS_WRAP,
		TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP, 0, 16, COMPARISON_NEVER);
	m_pAnisotropySampler16->Create(&params);

	return m_pAnisotropySampler16;
}

SamplerState* Renderer::LinearTiledSampler()
{
	if (m_pLinearTiledSampler)	return m_pLinearTiledSampler;

	//it's not created yet
	m_pLinearTiledSampler = new SamplerState();

	SamplerStateParams params;
	params.Init(FILTER_MIN_MAG_MIP_LINEAR, TEXTURE_ADDRESS_WRAP,
		TEXTURE_ADDRESS_WRAP, TEXTURE_ADDRESS_WRAP, 0, 16, COMPARISON_NEVER);
	m_pLinearTiledSampler->Create(&params);

	return m_pLinearTiledSampler;
}

SamplerState* Renderer::PointClampSampler()
{
	if (m_pPointClampSampler)	return m_pPointClampSampler;

	//notcreated yet
	m_pPointClampSampler = new SamplerState();

	SamplerStateParams params;
	params.Init(FILTER_MIN_MAG_MIP_POINT, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP,
		0, 16, COMPARISON_NEVER);
	m_pPointClampSampler->Create(&params);

	return m_pPointClampSampler;
}

SamplerState* Renderer::LinearLessEqualSampler()
{
	if (m_pLinearLessEqualSampler) return m_pLinearLessEqualSampler;

	//not created yet
	m_pLinearLessEqualSampler = new SamplerState();

	SamplerStateParams params;
	params.Init(FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, TEXTURE_ADDRESS_MIRROR, TEXTURE_ADDRESS_MIRROR, TEXTURE_ADDRESS_MIRROR,
		0, 16, COMPARISON_LESS_EQUAL);
	m_pLinearLessEqualSampler->Create(&params);

	return m_pLinearLessEqualSampler;
}

RasterizerState* Renderer::NoCullingStandardRasterizer()
{
	if (m_pNoCullingStandard) return m_pNoCullingStandard;

	//not created yet
	m_pNoCullingStandard = new RasterizerState();
	RasterizerParams params;
	params.Init(false, false, false, true, 0.0f, 0.0f, 0.0f, false, false, false, false);
	m_pNoCullingStandard->Create(&params);

	return m_pNoCullingStandard;
}

RasterizerState* Renderer::AllEnabledBackCullingRasterizer()
{
	if (m_pAllEnabledBackCulling) return m_pAllEnabledBackCulling;

	//not created yet
	m_pAllEnabledBackCulling = new RasterizerState();
	RasterizerParams params;
	params.Init(false, false, false, true, 0.0f, 0.0f, 0.0f, true, true, true, true);
	m_pAllEnabledBackCulling->Create(&params);

	return m_pAllEnabledBackCulling;
}

RasterizerState* Renderer::AllDisabledBackCullingRasterizer()
{
	if (m_pAllDisabledBackCulling) return m_pAllDisabledBackCulling;

	//not created yet
	m_pAllDisabledBackCulling = new RasterizerState();
	RasterizerParams params;
	params.Init(false, false, false, true, 0.0f, 0.0f, 0.0f, false, false, false, false);
	m_pAllDisabledBackCulling->Create(&params);

	return m_pAllDisabledBackCulling;
}

BlendState* Renderer::NoBlending()
{
	if (m_pNoBlending) return m_pNoBlending;

	//not created yet
	m_pNoBlending = new BlendState();
	RenderTargetBlendParams rtBlendParams;
	rtBlendParams.Init(false, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, 0x0f);
	BlendStateParams params;
	params.Init(false, false, &rtBlendParams, 1);

	m_pNoBlending->Create(&params);

	return m_pNoBlending;
}

BlendState* Renderer::BlendAddStandard()
{
	if (m_pBlendAddStandard) return m_pBlendAddStandard;

	//not created yet
	m_pBlendAddStandard = new BlendState();
	RenderTargetBlendParams rtBlendParams;
	rtBlendParams.Init(true, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, 0x0f);
	BlendStateParams params;
	params.Init(false, false, &rtBlendParams, 1);

	m_pBlendAddStandard->Create(&params);

	return m_pBlendAddStandard;
}

BlendState* Renderer::BlendLightPass()
{
	if (m_pBlendLightPass) return m_pBlendLightPass;

	//not created yet
	m_pBlendLightPass = new BlendState();
	RenderTargetBlendParams rtBlendParams;
	rtBlendParams.Init(true, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, 0x0f);
	BlendStateParams params;
	params.Init(false, false, &rtBlendParams, 1);

	m_pBlendLightPass->Create(&params);

	return m_pBlendLightPass;
}