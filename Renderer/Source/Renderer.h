#pragma once
//====================================================================================
//Renderer.h
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

#include "Cameras\Camera.h"
#include "Final/Resources/Buffers/Buffers.h"
#include "Final/States/SamplerState.h"
#include "Final/States/BlendState.h"
#include "Final/States/DepthStencilState.h"
#include "Final/States/RasterizerState.h"
#include "Meshes/Meshes.h"
#include "Lights/Light.h"

namespace Anubis
{
	class Renderer
	{
		friend class Mesh;
		friend class Light;
		friend class PointLight;
	public:
		/** ================================
				Constant Buffers
			================================
											*/

		//Matrix Buffers
		ConstantBuffer* m_pcbView;
		ConstantBuffer*	m_pcbWorldPlusWVP;
		ConstantBuffer* m_pcbWorldPlusWorldViewPlusWVP;
		ConstantBuffer* m_pcbWVP; 
		ConstantBuffer* m_pcbPrevViewProj;

		//Camera Buffers
		ConstantBuffer*	m_pcbCameraPos;
		ConstantBuffer*	m_pcbFrustumSize;
		ConstantBuffer* m_pcbCameraPosPlusInverseWP;

		//Lightning Buffers
		ConstantBuffer*	m_pcbLight;
		ConstantBuffer* m_pcbPointLight;
		ConstantBuffer* m_pcbSpotLight;
		//ConstantBuffer*	m_pcbDirectionalLight;

		//Depth Buffers
		ConstantBuffer*	m_pcbDepthBuffer;

		//SSAO Buffers
		ConstantBuffer*	m_pcbSSAOParams;
		ConstantBuffer*	m_pcbSSAOSamples;

		/** ================================
					Sampler States
			================================
											*/
		SamplerState*	m_pLinearTiledSampler;
		SamplerState*	m_pAnisotropySampler16;
		SamplerState*	m_pPointClampSampler;
		SamplerState*	m_pLinearLessEqualSampler;

		/** ================================
				  Rasterizer States
			================================
											*/
		RasterizerState* m_pAllEnabledBackCulling;
		RasterizerState* m_pNoCullingStandard;
		RasterizerState* m_pAllDisabledBackCulling;

		/** ================================
				 Blend States
			================================
											*/
		BlendState* m_pNoBlending;
		BlendState* m_pBlendAddStandard;
		BlendState* m_pBlendLightPass;

		/** ================================
					Depth Stencil States
			================================
											**/
		DepthStencilState*	m_pDepthEnableStencilDisableStandard;
		DepthStencilState*	m_pDepthDisableStencilDisable;
		Texture2D*			m_pDepthTexture;
		ShaderResourceView*	m_pDepthSRV;
		DepthStencilView*	m_pDepthDSV;

		/*** Linear Depth Buffer ***/
		//Texture2D*			m_pLinearDepthTexture;
		//ShaderResourceView*	m_pLinearDepthSRV;
		//RenderTargetView*	m_pLinearDepthRTV;

		/*** ========================
					Cameras
			 ======================== ***/
		Cameras	m_cameras;

		/*** =========================
			   Rendering Parameteres
		     ========================= ***/
		ABOOL	m_bTexturingOn;
		ABOOL	m_bLightningOn;

		/*** =========================
				Blurring
			 ========================= ***/
		enum FilterType
		{
			FT_Gaussian,
			FT_GaussianSeparable,
			FT_Bilateral,
		};
		ComputeShader* m_pGaussianFilter;
		ComputeShader* m_pGaussianHorFilter;
		ComputeShader* m_pGaussianVerFilter;

		ComputeShader* m_pCopyResourceShader;

		const ASTATIC ABOOL m_bVarianceShadows = true;

		//========================================/
		//Other
		Texture2D* m_pTempTexture;
		ShaderResourceView* m_pTempSRV;
		RenderTargetView* m_pTempRTV;
		UnorderedAccessView* m_pTempUAV;

		Texture2D* m_pVelocityTexture;
		ShaderResourceView* m_pVelocitySRV;
		RenderTargetView* m_pVelocityRTV;

	public:

		//constructor
		Renderer();
		~Renderer();

		/***	Initialize	  ***/
		AVIRTUAL ABOOL VInitialize(HWND hWnd, AUINT32 width, AUINT32 height);

		/***	Put something to render	***/
		AVIRTUAL AVOID VPushMesh(Mesh * pMesh) = 0;
		AVIRTUAL AVOID VPushLight(Light * pLight) = 0;

		/***	Add or Remove camera	***/
		AVOID AddCamera(CameraPtr pCamera);
		AVOID RemoveCamera(CameraPtr pCamera);

		/***	Update Render State		***/
		AVIRTUAL AVOID VUpdate(const AUINT32 deltaMilliseconds) = 0;

		/***	Render Scene	***
		**************************/
		AVIRTUAL AVOID VRender() = 0;
		AVOID FilterImage(ShaderResourceView* pSRV, UnorderedAccessView* pResultUAV, ShaderResourceView* pTempSRV, UnorderedAccessView* pTempUAV,
							AREAL texWidth, AREAL texHeight, FilterType type);
		AVIRTUAL AVOID VGenerateShadowMaps() = 0;

		//Accessors
		ABOOL LightningOn() const { return m_bLightningOn; }
		ABOOL TexturingOn() const { return m_bTexturingOn; }

		SamplerState * AnisotropySampler16();
		SamplerState * LinearTiledSampler();
		SamplerState * PointClampSampler();
		SamplerState * LinearLessEqualSampler();

		RasterizerState * NoCullingStandardRasterizer();
		RasterizerState * AllEnabledBackCullingRasterizer();
		RasterizerState * AllDisabledBackCullingRasterizer();

		BlendState* BlendLightPass();
		BlendState* BlendAddStandard();
		BlendState* NoBlending();
	};

}; //Anubis