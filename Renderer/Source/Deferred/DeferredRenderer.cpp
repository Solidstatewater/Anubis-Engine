//====================================================================================
//DeferredRenderer.h : deferred renderer implementation
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
#include "DeferredRenderer.h"

#include "Final\General.h"
#include "Final\Resources\ResourceData.h"

#include "../../Math/Source/Interpolation.h"

using namespace Anubis;

const AINT8 DeferredRenderer::s_i8SSAOKernelSize = 20;
const AINT8 DeferredRenderer::s_i8NoiseSize = 4;

DeferredRenderer::DeferredRenderer() : Renderer()
{
	m_pVertices		= new VertexBuffer();
	m_pTexCoords	= new VertexBuffer();

	m_pDefaultGBufferShaders = new ShaderBunch();

	//create matrix constant buffer
	m_pMatrixBuffer = new ConstantBuffer();

	m_pSSAOShaders = new ShaderBunch();
	m_pSSAOTexture = new Texture2D();     
	m_pSSAOSRV = new ShaderResourceView();
	m_pSSAOUAV = new UnorderedAccessView();
	m_pSSAORTV = new RenderTargetView();

	m_pSSAOBlurredTexture = new Texture2D();
	m_pSSAOBlurredSRV = new ShaderResourceView();
	m_pSSAOBlurredUAV = new UnorderedAccessView();

	m_pShadowsShaders = new ShaderBunch();
	m_pVarianceShadowsShaders = new ShaderBunch();

	m_pVelocityMapShaders = new ShaderBunch();
	m_pMotionBlurShaders = new ShaderBunch();

	m_pSRVtoRTVShaders = new ShaderBunch();
}

DeferredRenderer::~DeferredRenderer()
{
	//remove all lights
	while(!m_lights.empty())
		m_lights.pop_back();

	SAFE_DELETE(m_pDefaultLayout);
	SAFE_DELETE(m_pDefaultGBufferShaders);

	SAFE_DELETE(m_pSSAOShaders);
	SAFE_DELETE(m_pSSAOLayout);

	SAFE_DELETE(m_pSSAOBlurredUAV);
	SAFE_DELETE(m_pSSAOBlurredSRV);
	SAFE_DELETE(m_pSSAOSRV);
	SAFE_DELETE(m_pSSAOUAV);
	SAFE_DELETE(m_pSSAORTV);

	SAFE_DELETE(m_pSSAOTexture);
	SAFE_DELETE(m_pSSAOBlurredTexture);

	SAFE_DELETE(m_pShadowsShaders);
	SAFE_DELETE(m_pShadowsLayout);

	SAFE_DELETE(m_pVarianceShadowsShaders);

	SAFE_DELETE(m_pVelocityMapShaders);
	SAFE_DELETE(m_pMotionBlurShaders);
	SAFE_DELETE(m_pMotionBlurLayout);

	SAFE_DELETE(m_pLayout);
	SAFE_DELETE(m_pMatrixBuffer);
	SAFE_DELETE(m_pTexCoords);
	SAFE_DELETE(m_pVertices);

	SAFE_DELETE(m_pSRVtoRTVLayout);
	SAFE_DELETE(m_pSRVtoRTVShaders);
}

ABOOL DeferredRenderer::VInitialize(HWND hWnd, AUINT32 width, AUINT32 height)
{
	if (!Renderer::VInitialize(hWnd, width, height))
	{
		assert(0 && "Some problem initializing Renderer occured");
	}

	//initialize gbuffer
	m_gbuffer.VInitialize();

	//initialize data for light pass
	BufferParams matBufferParams;
	matBufferParams.FillConstantBufferParams(sizeof(Mat4x4), true, false, false);
	m_pMatrixBuffer->Create(&matBufferParams, NULL);

	//initialize data for light rendering
	BufferParams params;
	params.FillVertexBufferParams(sizeof(float3), 6, true, false, false, false);

	//position data
	SubresourceData data;

	AREAL32 left = (AREAL32)(SCREEN_WIDTH / 2 * -1);
	AREAL32 right = left + SCREEN_WIDTH;
	AREAL32 top = (AREAL32)(SCREEN_HEIGHT / 2);
	AREAL32 bottom = top - SCREEN_HEIGHT;

	float3 pos [] = 
	{
		float3(left,	top,	0.0f), //left top
		float3(right,	top,	0.0f), //right top
		float3(left,	bottom, 0.0f), //left bottom
		float3(left,	bottom, 0.0f), //left bottom
		float3(right,	top,	0.0f), //right top
		float3(right,	bottom, 0.0f), //right bottom
	};

	data.SetData(pos);
	if (!m_pVertices->Create(&params, &data, 6, sizeof(float3)))
	{
		assert(0 && "Error creating vertex buffer in Deferred Renderer initialization!");
	}

	//texture coordinates
	params.SetSize(sizeof(float2) * 6);

	float2 texCoords [] =
	{
		float2(0.0f, 0.0f), //left top
		float2(1.0f, 0.0f), //right top
		float2(0.0f, 1.0f), //left bottom
		float2(0.0f, 1.0f), //left bottom
		float2(1.0f, 0.0f), //right top
		float2(1.0f, 1.0f), //right bottom
	};

	data.SetData(texCoords);
	if (!m_pTexCoords->Create(&params, &data, 6, sizeof(float2)))
	{
		assert(0 && "Error creating vertex buffer with texture coordinates in Deferred Renderer initialization!");
	}

	//initialize input layout 
	m_pDefaultLayout = new INPUT_LAYOUT[5];

	m_pDefaultLayout[0].SemanticName			= "POSITION";			m_pDefaultLayout[1].SemanticName			= "TEXCOORDS";
	m_pDefaultLayout[0].SemanticIndex			= 0;					m_pDefaultLayout[1].SemanticIndex			= 0;
	m_pDefaultLayout[0].Format					= TEX_R32G32B32_FLOAT;	m_pDefaultLayout[1].Format					= TEX_R32G32_FLOAT;
	m_pDefaultLayout[0].InputSlot				= 0;					m_pDefaultLayout[1].InputSlot				= 1;
	m_pDefaultLayout[0].AlignedByteOffset		= 0;					m_pDefaultLayout[1].AlignedByteOffset		= 0;
	m_pDefaultLayout[0].InputSlotClass			= IA_PER_VERTEX_DATA;	m_pDefaultLayout[1].InputSlotClass			= IA_PER_VERTEX_DATA;
	m_pDefaultLayout[0].InstanceDataStepRate	= 0;					m_pDefaultLayout[1].InstanceDataStepRate	= 0;

	m_pDefaultLayout[2].SemanticName			= "NORMAL";				m_pDefaultLayout[3].SemanticName			= "TANGENT";
	m_pDefaultLayout[2].SemanticIndex			= 0;					m_pDefaultLayout[3].SemanticIndex			= 0;
	m_pDefaultLayout[2].Format					= TEX_R32G32B32_FLOAT;	m_pDefaultLayout[3].Format					= TEX_R32G32B32_FLOAT;
	m_pDefaultLayout[2].InputSlot				= 2;					m_pDefaultLayout[3].InputSlot				= 2;
	m_pDefaultLayout[2].AlignedByteOffset		= 0;					m_pDefaultLayout[3].AlignedByteOffset		= 12;
	m_pDefaultLayout[2].InputSlotClass			= IA_PER_VERTEX_DATA;	m_pDefaultLayout[3].InputSlotClass			= IA_PER_VERTEX_DATA;
	m_pDefaultLayout[2].InstanceDataStepRate	= 0;					m_pDefaultLayout[3].InstanceDataStepRate	= 0;

	m_pDefaultLayout[4].SemanticName			= "BINORMAL";			
	m_pDefaultLayout[4].SemanticIndex			= 0;					
	m_pDefaultLayout[4].Format					= TEX_R32G32B32_FLOAT;	
	m_pDefaultLayout[4].InputSlot				= 2;					
	m_pDefaultLayout[4].AlignedByteOffset		= 24;					
	m_pDefaultLayout[4].InputSlotClass			= IA_PER_VERTEX_DATA;	
	m_pDefaultLayout[4].InstanceDataStepRate	= 0;					

	//Initialize shaders
	m_pDefaultGBufferShaders->VSetVertexShader(L"GBufferShader.hlsl", "VS", m_pDefaultLayout, 5, TOPOLOGY_TRIANGLELIST);
	m_pDefaultGBufferShaders->VSetPixelShader(L"GBufferShader.hlsl", "PS");

	//input layout
	m_pLayout = new INPUT_LAYOUT[2];
	m_pLayout[0].SemanticName			= "POSITION";			m_pLayout[1].SemanticName			= "TEXCOORDS";
	m_pLayout[0].SemanticIndex			= 0;					m_pLayout[1].SemanticIndex			= 0;
	m_pLayout[0].Format					= TEX_R32G32B32_FLOAT;	m_pLayout[1].Format					= TEX_R32G32_FLOAT;
	m_pLayout[0].InputSlot				= 0;					m_pLayout[1].InputSlot				= 1;
	m_pLayout[0].AlignedByteOffset		= 0;					m_pLayout[1].AlignedByteOffset		= 0;
	m_pLayout[0].InputSlotClass			= IA_PER_VERTEX_DATA;	m_pLayout[1].InputSlotClass			= IA_PER_VERTEX_DATA;
	m_pLayout[0].InstanceDataStepRate	= 0;					m_pLayout[1].InstanceDataStepRate	= 0;

	////////////////////////////////////////////////////////
	//Shadow mapping
	m_pShadowsLayout = new INPUT_LAYOUT();
	m_pShadowsLayout->SemanticName	=	"POSITION";
	m_pShadowsLayout->SemanticIndex	=	0;
	m_pShadowsLayout->Format		=	TEX_R32G32B32_FLOAT;
	m_pShadowsLayout->InputSlot		=	0;
	m_pShadowsLayout->AlignedByteOffset =	0;
	m_pShadowsLayout->InputSlotClass	=	IA_PER_VERTEX_DATA;
	m_pShadowsLayout->InstanceDataStepRate =	0;

	m_pShadowsShaders->VSetVertexShader(L"Resources\\Shaders\\Shadows\\ShadowMap_VS.hlsl", "Shadows_VS", m_pShadowsLayout, 1, TOPOLOGY_TRIANGLELIST);
	m_pShadowsShaders->VSetPixelShader(L"Resources\\Shaders\\Shadows\\ShadowMap_PS.hlsl", "Shadows_PS");
	
	m_pVarianceShadowsShaders->VSetVertexShader(L"Resources\\Shaders\\Shadows\\VarianceShadows_DepthVS.hlsl", "VarianceShadows_DepthVS", m_pShadowsLayout, 1, TOPOLOGY_TRIANGLELIST);
	m_pVarianceShadowsShaders->VSetPixelShader(L"Resources\\Shaders\\Shadows\\VarianceShadows_DepthPS.hlsl", "VarianceShadows_DepthPS");

	////////////////////////////////////////////////////////
	//Motion Blur
	m_pMotionBlurLayout = new INPUT_LAYOUT[2];
	m_pMotionBlurLayout[0].SemanticName = "POSITION";				m_pMotionBlurLayout[1].SemanticName = "TEXCOORD";
	m_pMotionBlurLayout[0].SemanticIndex = 0;						m_pMotionBlurLayout[1].SemanticIndex = 0;
	m_pMotionBlurLayout[0].Format = TEX_R32G32B32_FLOAT;			m_pMotionBlurLayout[1].Format = TEX_R32G32_FLOAT;
	m_pMotionBlurLayout[0].InputSlot = 0;							m_pMotionBlurLayout[1].InputSlot = 1;
	m_pMotionBlurLayout[0].AlignedByteOffset = 0;					m_pMotionBlurLayout[1].AlignedByteOffset = 0;
	m_pMotionBlurLayout[0].InputSlotClass = IA_PER_VERTEX_DATA;		m_pMotionBlurLayout[1].InputSlotClass = IA_PER_VERTEX_DATA;
	m_pMotionBlurLayout[0].InstanceDataStepRate = 0;				m_pMotionBlurLayout[1].InstanceDataStepRate = 0;

	m_pVelocityMapShaders->VSetVertexShader(L"Resources\\Shaders\\PostProcessing\\VelocityMap_VS.hlsl", "GenerateVelocityMap_VS", m_pMotionBlurLayout, 2, TOPOLOGY_TRIANGLELIST);
	m_pVelocityMapShaders->VSetPixelShader(L"Resources\\Shaders\\PostProcessing\\VelocityMap_PS.hlsl", "GenerateVelocityMap_PS");

	m_pMotionBlurShaders->VSetVertexShader(L"Resources\\Shaders\\PostProcessing\\MotionBlur_VS.hlsl", "MotionBlur_VS", m_pMotionBlurLayout, 2, TOPOLOGY_TRIANGLELIST);
	m_pMotionBlurShaders->VSetPixelShader(L"Resources\\Shaders\\PostProcessing\\MotionBlur_PS.hlsl", "MotionBlur_PS");

	////////////////////////////////////////////////////////
	//resourece copying
	m_pSRVtoRTVLayout = new INPUT_LAYOUT();
	m_pSRVtoRTVLayout->SemanticName		=	"POSITION";
	m_pSRVtoRTVLayout->SemanticIndex	=	0;
	m_pSRVtoRTVLayout->Format			=	TEX_R32G32B32_FLOAT;
	m_pSRVtoRTVLayout->InputSlot		=	0;
	m_pSRVtoRTVLayout->AlignedByteOffset =	0;
	m_pSRVtoRTVLayout->InputSlotClass	=	IA_PER_VERTEX_DATA;
	m_pSRVtoRTVLayout->InstanceDataStepRate =	0;

	m_pSRVtoRTVShaders->VSetVertexShader(L"Resources\\Shaders\\Other\\FromSRVtoRTV_VS.hlsl", "SRVtoRTV_VS", m_pSRVtoRTVLayout, 1, TOPOLOGY_TRIANGLELIST);
	m_pSRVtoRTVShaders->VSetPixelShader(L"Resources\\Shaders\\Other\\FromSRVtoRTV_PS.hlsl", "SRVtoRTV_PS");

	////////////////////////////////////////////////////////
	//Ambient occlusion
	m_pSSAOLayout = new INPUT_LAYOUT();
	m_pSSAOLayout->SemanticName		=	"POSITION";
	m_pSSAOLayout->SemanticIndex	=	0;
	m_pSSAOLayout->Format			=	TEX_R32G32B32_FLOAT;
	m_pSSAOLayout->InputSlot		=	0;
	m_pSSAOLayout->AlignedByteOffset =	0;
	m_pSSAOLayout->InputSlotClass	=	IA_PER_VERTEX_DATA;
	m_pSSAOLayout->InstanceDataStepRate =	0;

	m_pSSAOShaders->VSetVertexShader(L"SSAO_Vertex.hlsl", "SSAO_VS", m_pSSAOLayout, 1, TOPOLOGY_TRIANGLELIST);
	m_pSSAOShaders->VSetPixelShader(L"SSAO_Pixel.hlsl", "SSAO_PS");

	Texture2DParams tex2DParams;
	tex2DParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, TEX_R32G32B32A32_FLOAT, true, true, true, false, 1, 0,
		1, true, false, false);
	m_pSSAOTexture->Create(&tex2DParams);

	//create shader resource and render target view for the texture
	ShaderResourceViewParams srvSSAOParams;
	srvSSAOParams.InitForTexture2D(tex2DParams.Format, 0, 0);
	m_pSSAOTexture->CreateShaderResourceView(&m_pSSAOSRV->m_pView, &srvSSAOParams);

	UnorderedAccessViewParams uavSSAOParams;
	uavSSAOParams.InitForTexture2D(tex2DParams.Format, 0);
	m_pSSAOTexture->CreateUnorderedAccessView(&m_pSSAOUAV->m_pView, &uavSSAOParams);

	RenderTargetViewParams rtvSSAOParams;
	rtvSSAOParams.InitForTexture2D(tex2DParams.Format, 0);
	m_pSSAOTexture->CreateRenderTargetView(&m_pSSAORTV->m_pView, &rtvSSAOParams);

	///////////////////////////////////////////////
	//Initialize data for blurring
	///////////////////////////////////////////////
	tex2DParams.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 1, TEX_R32G32B32A32_FLOAT, true, true, false, false, 1, 0,
		1, true, false, false);
	m_pSSAOBlurredTexture->Create(&tex2DParams);

	srvSSAOParams.InitForTexture2D(tex2DParams.Format, 0, 0);
	m_pSSAOBlurredTexture->CreateShaderResourceView(&m_pSSAOBlurredSRV->m_pView, &srvSSAOParams);
	
	uavSSAOParams.InitForTexture2D(tex2DParams.Format, 0);
	m_pSSAOBlurredTexture->CreateUnorderedAccessView(&m_pSSAOBlurredUAV->m_pView, &uavSSAOParams);

	return true;
}

///////////////////////////////////////////////
//Updating
///////////////////////////////////////////////
AVOID DeferredRenderer::VPushMesh(Mesh * pMesh)
{
	m_queue.VAdd(pMesh);
}

AVOID DeferredRenderer::VPushLight(Light * pLight)
{
	m_lights.push_back(pLight);
}

AVOID DeferredRenderer::VUpdate(const AUINT32 deltaMilliseconds)
{
	//Firstly, sort render queue
	m_queue.VSort();
}

///////////////////////////////////////////////
//Ambient occlusion
///////////////////////////////////////////////
AVOID DeferredRenderer::CalculateAmbientOcclusion(CameraPtr pCamera)
{
	m_pSSAOShaders->VBind();

	m_pVertices->Set(0, 0);

	//bind matrix constant buffer to the pipeline
	Mat4x4 trans;
	trans.CreateTranslation(pCamera->GetLookAt());

	Mat4x4 rot;
	rot = rot.CreateRollPitchYaw(pCamera->GetRoll(), pCamera->GetPitch(), pCamera->GetYaw());

	Mat4x4 WVP = rot * trans * pCamera->GetView() * CreateOrthoProjectionLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.0f);
	WVP.Transpose();
	m_pMatrixBuffer->UpdateSubresource(0, NULL, &WVP, 0, 0);
	m_pMatrixBuffer->Set(0, ST_Vertex);

	//camera params
	Vec cameraParams = Vector(pCamera->GetFrustum().GetNearWidth(), pCamera->GetFrustum().GetNearHeight(), 0.0f, 0.0f);
	m_pcbFrustumSize->UpdateSubresource(0, NULL, &cameraParams, 0, 0);
	m_pcbFrustumSize->Set(0, ST_Pixel);

	//depth buffer params
	Vec depthBufferParams = Vector(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0.0f, 0.0f);
	m_pcbDepthBuffer->UpdateSubresource(0, NULL, &depthBufferParams, 0, 0);
	m_pcbDepthBuffer->Set(1, ST_Pixel); 

	//camera view matrix
	Mat4x4 View = pCamera->GetView();
	View.Transpose();
	m_pcbView->UpdateSubresource(0, NULL, &View, 0, 0);
	m_pcbView->Set(2, ST_Pixel);

	//send SSAO params
	//Vec ssaoParams = Vector(20.0f, 2.0f, 5.0f, 2.0f);
	//m_pcbSSAOParams->UpdateSubresource(0, NULL, &ssaoParams, 0, 0);
	//m_pcbSSAOParams->Set(2, ST_Pixel);
	
	m_pDepthDisableStencilDisable->Set(0);
	//m_pDepthSRV->Set(0, ST_Pixel);
	m_gbuffer.m_SRVList.SetView(2, 0, ST_Pixel);
	m_gbuffer.m_SRVList.SetView(5, 1, ST_Pixel);
	m_pSSAORTV->Set();
	
	//set sampler states
	//AnisotropySampler16()->Set(0, ST_Pixel);
	PointClampSampler()->Set(0, ST_Pixel);
	D3D11DeviceContext()->Draw(6, 0);

	//Unbind render target view
	UnbindRenderTargetViews(1);

	//m_pSSAOSRV->Set(5, ST_Pixel); 
	UnbindShaderResourceViews(0, 2, ST_Pixel);
	GetRenderTargetView()->Set(); 
}

///////////////////////////////////////////////
//Shadowing
///////////////////////////////////////////////
AVOID DeferredRenderer::VGenerateShadowMaps()
{
	if (m_bVarianceShadows)
	{
		m_pVarianceShadowsShaders->VBind();
		//m_pShadowsShaders->VBind();
	}
	else
	{
		m_pShadowsShaders->VBind();
	}

	//Set rendering states
	m_pDepthEnableStencilDisableStandard->Set(1);
	for (LightList::iterator lit = m_lights.begin(); lit != m_lights.end(); lit++)
	{
		Light* pLight = (*lit);

		//initialize light if it hasn't been yet 
		if (!pLight->IsInitialized())
			pLight->VInitialize(m_pLayout);

		//clear shadow map before generating new one
		pLight->ClearShadowMap();

		Mesh * pMesh;
		m_queue.Reset();
		while (pMesh = m_queue.Next())
		{
			//pMesh->VPreRender(this, view, viewProj); 
			pLight->VPrepareToGenerateShadowMap(pMesh->GetWorldTransform(), this);
			pMesh->GenerateShadowMap();
		} 

		pLight->VFinishShadowMapGeneration(this);
	}
}

///////////////////////////////////////////////
//Motion Blur
///////////////////////////////////////////////
AVOID DeferredRenderer::VGenerateVelocityMap(CameraPtr pCamera, Renderer* pRenderer, const Mat4x4 & viewproj)
{
	//set screen-aligned quad
	m_pVertices->Set(0, 0);

	//set shaders
	m_pVelocityMapShaders->VBind();

	//set shader resources
	m_gbuffer.BindPositionTex(0, ST_Pixel);

	//set render target
	m_pVelocityRTV->Set();

	//set constant buffers

	//bind matrix constant buffer to the pipeline
	Mat4x4 trans;
	//trans.CreateTranslation(pCamera->GetLookAt() + 10000 * pCamera->GetDir());
	trans.CreateTranslation(pCamera->GetPosition() + 500* pCamera->GetDir());

	Mat4x4 rot;
	rot = rot.CreateRollPitchYaw(pCamera->GetRoll(), pCamera->GetPitch(), pCamera->GetYaw());

	Mat4x4 WVP = rot * trans * pCamera->GetView() * CreateOrthoProjectionLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1000.0f);
	WVP.Transpose();
	m_pMatrixBuffer->UpdateSubresource(0, NULL, &WVP, 0, 0);
	m_pMatrixBuffer->Set(0, ST_Vertex);

	//set previous frame view-projection transform
	Mat4x4 currViewProj = viewproj;
	Mat4x4 prevViewProj = pCamera->GetPrevView() * pCamera->GetPrevProjection();
	//prevViewProj = pCamera->GetView() * pCamera->GetProjection();
	//prevViewProj = pCamera->GetProjection();
	currViewProj.Transpose();
	prevViewProj.Transpose();
	//m_pcbPrevViewProj->UpdateSubresource(0, NULL, &prevViewProj, 0, 0);
	//m_pcbPrevViewProj->Set(0, ST_Pixel);

	struct TwoMatrices
	{
		Mat4x4 prevViewProj;
		Mat4x4 currViewProj;
	};
	TwoMatrices temp;
	temp.prevViewProj =prevViewProj;
	temp.currViewProj = currViewProj;

	m_pcbWorldPlusWVP->UpdateSubresource(0, nullptr, &temp, 0, 0);
	m_pcbWorldPlusWVP->Set(0, ST_Pixel);

	//Finally render velocity map
	Draw(6, 0);

	//unbind views
	UnbindShaderResourceViews(0, 1, ST_Pixel);
	UnbindRenderTargetViews(1);
}

///////////////////////////////////////////////
//Rendering
///////////////////////////////////////////////
AVOID DeferredRenderer::GenerateSSAOKernel()
{
	for (AINT8 i = 0; i < s_i8SSAOKernelSize; i++)
	{
		m_vSSAOKernel[i] = Vector(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), Random(-1.0f, 0.0f), 0.0f);
		m_vSSAOKernel[i] = Normalize(m_vSSAOKernel[i]);

		//more density at smaller radius
		AREAL r32Scale = AREAL(i) / AREAL(s_i8SSAOKernelSize);
		r32Scale = LERP(0.1f, 1.0f, r32Scale * r32Scale);
		m_vSSAOKernel[i] = m_vSSAOKernel[i] * r32Scale;
	}
}

AVOID DeferredRenderer::GenerateNoiseTexture()
{
	for (AINT8 i = 0; i < s_i8NoiseSize; i++)
	{
		m_vNoise[i] = Vector(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), 0.0f, 0.0f);
		Normalize(m_vNoise[i]);
	}
}

AVOID DeferredRenderer::VSetStateForGBuffer()
{
	m_pDefaultGBufferShaders->VBind();
}

AVOID DeferredRenderer::PrepareForLightPass(CameraPtr pCamera)                                                                                                                                                                                                                                       
{
	//set vertex buffer with positions
	m_pVertices->Set(0, 0);

	//set vertex buffer with texture data
	m_pTexCoords->Set(1, 0);

	//bind matrix constant buffer to the pipeline
	Mat4x4 trans;
	//trans.CreateTranslation(pCamera->GetLookAt() + 10000 * pCamera->GetDir());
	trans.CreateTranslation(pCamera->GetPosition() + 500* pCamera->GetDir());

	Mat4x4 rot;
	rot = rot.CreateRollPitchYaw(pCamera->GetRoll(), pCamera->GetPitch(), pCamera->GetYaw());

	Mat4x4 WVP = rot * trans * pCamera->GetView() * CreateOrthoProjectionLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1000.0f);
	WVP.Transpose();
	m_pMatrixBuffer->UpdateSubresource(0, NULL, &WVP, 0, 0);
	m_pMatrixBuffer->Set(0, ST_Vertex);

	struct CameraBuffer
	{
		Mat4x4 inverseViewProjection;
		Vec pos;
	};
	CameraBuffer cameraBuffer;
	Mat4x4 inverseViewProjection = pCamera->GetViewProjection();
	inverseViewProjection.Inverse();
	cameraBuffer.pos = pCamera->GetPosition();
	cameraBuffer.inverseViewProjection = inverseViewProjection;
	cameraBuffer.inverseViewProjection.Transpose();

	m_pcbCameraPos->UpdateSubresource(0, nullptr, &pCamera->GetPosition(), 0, 0);
	//m_pcbCameraPos->UpdateSubresource(0, NULL, &cameraBuffer, 0, 0);
	m_pcbCameraPos->Set(0, ST_Pixel);

	//set shader resources
	m_pSSAOBlurredSRV->Set(6, ST_Pixel);

	//set blending functionality
	//this->BlendLightPass()->Set(nullptr);
}

AVOID DeferredRenderer::PrepareForGeometryPass(CameraPtr pCamera)
{
	m_pcbCameraPos->UpdateSubresource(0, NULL, &pCamera->GetPosition(), 0, 0);
	m_pcbCameraPos->Set(0, ST_Pixel);
}

AVOID DeferredRenderer::VFinishPass()
{
	//clean g-buffer
	m_gbuffer.Clear();

	//clean depth buffer
	ClearDepthStencilView(true, false, 1.0f, 0xFF, m_pDepthDSV);

	//Clean lights' views if necessary
	for (LightList::iterator lit = m_lights.begin(); lit != m_lights.end(); lit++)
	{
		Light* pLight = (*lit);
		pLight->ClearShadowMap();
	}
}

AVOID DeferredRenderer::VRender()
{
	/*** Render Scene For Each Camera ***/
 	//for (CameraPtr pCamera : m_cameras)
	for (Cameras::iterator it = m_cameras.begin(); it != m_cameras.end(); it++)
	{
		CameraPtr pCamera = (*it);
		//Keep track of current view and projection matrices
		Mat4x4 view			= pCamera->GetView();
		Mat4x4 projection	= pCamera->GetProjection();
		Mat4x4 viewProj		= view * projection; //calculate view * projection matrix

		//	Rendering to g-buffer
		m_gbuffer.BindForWriting(m_pDepthDSV);

		// ===================================================== //
		//	Go through sorted render queue and render each mesh  //
		// ===================================================== //
		PrepareForGeometryPass(pCamera);
		VSetStateForGBuffer();
		m_pDepthEnableStencilDisableStandard->Set(1);
		Mesh * pMesh;
		m_queue.Reset();
		while (pMesh = m_queue.Next())
		{
			//set states needed for rendering current mesh
			pMesh->VPreRender(this, view, viewProj); 

			//finally render it
			pMesh->VRender(this);

			//remove previous states
			pMesh->VPostRender(this);
		} 
		//unbind g-buffer views
		m_gbuffer.UnbindFromWriting();

		// ========================================= //
		//		Generate Shadow Maps				 //
		// ========================================= //
		VGenerateShadowMaps();

		// ========================================= //
		//		Generate velocity map				 //
		// ========================================= //
		VGenerateVelocityMap(pCamera, this, viewProj);

		//clear meshes queue
		m_queue.Clear();
		//m_pDepthDisableStencilDisable->Set(0);

		//filter variance shadow maps
		if (m_bVarianceShadows)
		{
			for (LightList::iterator lit = m_lights.begin(); lit != m_lights.end(); lit++)
			{
				Light* pLight = (*lit);
				for (int i =0; i < 5; i++)
				{
					FilterImage(pLight->GetVarianceShadowMapSRV(), pLight->GetVarianceShadowUAV(), pLight->GetTempSRV(), pLight->GetTempUAV(), SCREEN_WIDTH, SCREEN_HEIGHT, FT_Gaussian);
				}
			}
		}

		// ========================================= //
		//	Calculate ambient occlusion				 //
		// ========================================= //
		//CalculateAmbientOcclusion(pCamera);
		//FilterImage(m_pSSAOSRV, m_pSSAOBlurredUAV, FT_Gaussian);

		//m_pSSAOSRV->Set(5, ST_Pixel); 
		// ========================================= //
		//	Time for light pass - Render all lights  //
		// ========================================= //

		//set g-buffer for reading and set back buffer as render target
		m_gbuffer.BindForReading(0);
		//SetRenderTargetView(); 
		m_pTempRTV->Set();

		PrepareForLightPass(pCamera);

		//for (Light * pLight : m_lights)
		for (LightList::iterator lit = m_lights.begin(); lit != m_lights.end(); lit++)
		{
			Light* pLight = (*lit);

			//initialize light if it hasn't been yet 
			if (!pLight->IsInitialized())
				pLight->VInitialize(m_pLayout);

			//set states needed for rendering current light
			pLight->VPreRender(this); 

			//scissor test optimization
			pLight->VSetScissorRect(this, pCamera);

			//finally render it
			pLight->VRender();

			//remove all used states
			pLight->VPostRender(this);
		}
		this->NoBlending()->Set(nullptr);

		VApplyMotionBlur();
		//CopyPostProcessingToBackBuffer();

		//clean lights
		while(!m_lights.empty())
		{
			m_lights.pop_back();
		}

		//unbind g-buffer views
		m_gbuffer.UnbindFromReading(0);
		m_gbuffer.Clear();

		UnbindShaderResourceViews(6, 1, ST_Pixel);
		VFinishPass();
	}
}

AVOID DeferredRenderer::VApplyMotionBlur()
{
	SetRenderTargetView();
	m_pVertices->Set(0, 0);
	m_pTexCoords->Set(1, 0);

	m_pTempSRV->Set(0, ST_Pixel);
	m_pVelocitySRV->Set(1, ST_Pixel);
	//m_pSRVtoRTVShaders->VBind();
	m_pMotionBlurShaders->VBind();

	Draw(6, 0);

	UnbindShaderResourceViews(0, 1, ST_Pixel);
	AREAL bgColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	ClearRenderTargetView(bgColor, m_pTempRTV); 

	AREAL velColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	ClearRenderTargetView(velColor, m_pVelocityRTV);
}

AVOID DeferredRenderer::CopyPostProcessingToBackBuffer()
{
	SetRenderTargetView();
	m_pTempSRV->Set(0, ST_Pixel);
	m_pSRVtoRTVShaders->VBind();

	Draw(6, 0);

	UnbindShaderResourceViews(0, 1, ST_Pixel);
	AREAL bgColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	ClearRenderTargetView(bgColor, m_pTempRTV); 
}