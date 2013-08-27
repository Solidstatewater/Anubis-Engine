#pragma once
//====================================================================================
//DeferredRenderer.h
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

#include "../Renderer.h"
#include "GBuffer.h"

#include "../Other/ShaderBunch.h"
#include "../Cameras/Camera.h"
#include "../RenderQueue.h"

#include "../Lights/DirectionalLight.h"
#include "../Lights/PointLight.h"
#include "../Lights/SpotLight.h"

#include "Final\Resources\Textures\Texture2D.h"
#include "Final\Resources\Views\Views.h"

#include "../../Math/Source/Vectors.h"

namespace Anubis
{

	class DeferredRenderer : public Renderer
	{
	protected:
		// ======================= //
		//	All Rendering Data === //
		// ======================= //
		GBufferOne	m_gbuffer;
		INPUT_LAYOUT* m_pDefaultLayout;
		ShaderBunch* m_pDefaultGBufferShaders;

		ConstantBuffer*	m_pMatrixBuffer;

		//meshes
		RenderQueue m_queue;

		//lights
		typedef vector<Light*> LightList;
		LightList	m_lights;

		//data for light rendering
		VertexBuffer*	m_pVertices;
		VertexBuffer*	m_pTexCoords;
		INPUT_LAYOUT*	m_pLayout;

		//===============================
		//Ambient occlusion
		INPUT_LAYOUT*	m_pSSAOLayout;
		ShaderBunch*	m_pSSAOShaders;
		Texture2D*		m_pSSAOTexture;
		ShaderResourceView*	m_pSSAOSRV;
		UnorderedAccessView* m_pSSAOUAV;
		RenderTargetView*	m_pSSAORTV;

		Texture2D*		m_pSSAOBlurredTexture;
		ShaderResourceView*	m_pSSAOBlurredSRV;
		UnorderedAccessView*	m_pSSAOBlurredUAV;

		Texture2D*		m_pNoiseTexture;
		ShaderResourceView* m_pNoiseSRV;
		UnorderedAccessView* m_pNoiseUAV;

		static const AINT8 s_i8SSAOKernelSize;
		static const AINT8 s_i8NoiseSize;

		Vec* m_vSSAOKernel;
		Vec* m_vNoise;

		//================================
		//Shadow mapping
		INPUT_LAYOUT* m_pShadowsLayout;
		ShaderBunch*	m_pShadowsShaders;

		////////////////////////////////////
		//Variance shadows
		ShaderBunch*	m_pVarianceShadowsShaders;

		////////////////////////////////////
		//Motion Blur
		INPUT_LAYOUT*	m_pMotionBlurLayout;
		ShaderBunch*	m_pVelocityMapShaders;
		ShaderBunch*	m_pMotionBlurShaders;


		INPUT_LAYOUT*	m_pSRVtoRTVLayout;
		ShaderBunch*	m_pSRVtoRTVShaders;

	public:
		/***		=====		***
		 Constructor and Destructor
		 ***		=====	   ***/
		DeferredRenderer();
		~DeferredRenderer();

		/***	Initialize	  ***/
		ABOOL VInitialize(HWND hWnd, AUINT32 width, AUINT32 height);

		/***	Put something to render	***/
		AVOID VPushMesh(Mesh * pMesh);
		AVOID VPushLight(Light * pLight);

		/***	Light Rendering ***/
		AVOID PrepareForLightPass(CameraPtr pCamera);

		/***	Ambient Occlusion	***/
		AVOID GenerateSSAOKernel();
		AVOID GenerateNoiseTexture();
		AVOID CalculateAmbientOcclusion(CameraPtr pCamera);

		/***	Shadows	***/
		AVOID VGenerateShadowMaps();

		/***	Post-processing ***/
		//Motion blur
		AVOID VGenerateVelocityMap(CameraPtr pCamera, Renderer* pRenderer, const Mat4x4 & viewproj);
		AVOID VApplyMotionBlur();

		/***	Update Render State		***/
		AVOID VUpdate(const AUINT32 deltaMilliseconds);

		/***	Render Scene	***
		**************************/
		AVOID VRender();
		AVOID VSetStateForGBuffer();
		AVOID PrepareForGeometryPass(CameraPtr pCamera);
		AVOID VSetStateForFinalPass();
		AVOID VFinishPass();
		AVOID CopyPostProcessingToBackBuffer();
	};
}; //Anubis