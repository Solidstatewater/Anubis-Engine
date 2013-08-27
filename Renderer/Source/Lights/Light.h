#pragma once
//====================================================================================
//Light.h
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

#include "../Other/ShaderBunch.h"
#include "../../Math/Source/Vectors.h"
#include "../../Math/Source/Mat4x4.h"
#include "Final\Resources\Buffers\BufferParams.h"
#include "Final\Resources\Buffers\Buffers.h"
#include "../Renderer_pch.h"

#include "Final\Resources\Textures\Texture2D.h"
#include "Final\Resources\Views\Views.h"

#include "../Cameras/Camera.h"
namespace Anubis
{
	//Light Types
	enum LightType
	{
		LT_Point		= 1,
		LT_Spot			= 2,
		LT_Directional	= 3,
	};

	/************************************************************
	============= Class with basic light properties =============
	************************************************************/
	class Renderer;
	class Light
	{
	protected:
		ABOOL m_bInitialized;

		Mat4x4 m_worldTransform;

		/*** Light Shader ***/
		AWSTRING m_vertexShaderFile;
		AWSTRING m_pixelShaderFile;
		ShaderBunch*	m_pShaders;

		/*** Constant Buffer With Light Data ***/
		//ConstantBuffer*	m_pBuffer; 

		/**				
			== Light Data ==
								**/
	
		struct LightData //define structure with light properties
		{
			Vec m_color;
			Vec m_pos;
			Vec m_dir;
		};
		AREAL32	m_r32Range;

		LightData* m_pData;

		///////////////////////////////////
		//Shadows
		///////////////////////////////////

		/* Percentage-closer soft shadows */
		Texture2D*	m_pShadowMapTex;
		ShaderResourceView*	m_pShadowMapSRV;
		DepthStencilView*	m_pShadowMapDSV;

		/* Variance shadow maps */
		Texture2D* m_pVarianceShadowTex;
		ShaderResourceView* m_pVarianceShadowSRV;
		RenderTargetView* m_pVarianceShadowRTV;
		UnorderedAccessView* m_pVarianceShadowUAV;

		Texture2D* m_pTempTexture;
		ShaderResourceView* m_pTempSRV;
		UnorderedAccessView* m_pTempUAV;

		Mat4x4				m_view;
		Mat4x4				m_viewProjection;

		AINT32				m_iShadowMapWidth;
		AINT32				m_iShadowMapHeight;
	public:

		//Constructor
		Light(const AWSTRING & vertexShaderFile = DEFAULT_LIGHT_PASS_VERTEX_SHADER_FILE, const AWSTRING & pixelShaderFile = DEFAULT_LIGHT_PASS_PIXEL_SHADER_FILE) : m_bInitialized(false)
		{
			m_pShaders = NULL;

			m_vertexShaderFile = vertexShaderFile;
			m_pixelShaderFile = pixelShaderFile;
			m_pShadowMapTex = new Texture2D();
			m_pShadowMapSRV = new ShaderResourceView();
			m_pShadowMapDSV = new DepthStencilView();

			m_pVarianceShadowTex = new Texture2D();
			m_pVarianceShadowSRV = new ShaderResourceView();
			m_pVarianceShadowRTV = new RenderTargetView();
			m_pVarianceShadowUAV = new UnorderedAccessView();

			m_pTempTexture = new Texture2D();
			m_pTempSRV = new ShaderResourceView();
			m_pTempUAV = new UnorderedAccessView();

			m_iShadowMapWidth = SCREEN_WIDTH / 48.0;
			m_iShadowMapHeight = SCREEN_HEIGHT / 48.0f;
		}
		Light(const Vec& color, const Vec& pos, const Vec& dir,
			const AWSTRING & vertexShaderFile = DEFAULT_LIGHT_PASS_VERTEX_SHADER_FILE, const AWSTRING & pixelShaderFile = DEFAULT_LIGHT_PASS_PIXEL_SHADER_FILE) : m_bInitialized(false)
		{
			m_pShaders = NULL;

			m_vertexShaderFile = vertexShaderFile;
			m_pixelShaderFile = pixelShaderFile;
			m_pShadowMapTex = new Texture2D();
			m_pShadowMapSRV = new ShaderResourceView();
			m_pShadowMapDSV = new DepthStencilView();

			m_pVarianceShadowTex = new Texture2D();
			m_pVarianceShadowSRV = new ShaderResourceView();
			m_pVarianceShadowRTV = new RenderTargetView();
			m_pVarianceShadowUAV = new UnorderedAccessView();

			m_pTempTexture = new Texture2D();
			m_pTempSRV = new ShaderResourceView();
			m_pTempUAV = new UnorderedAccessView();

			m_iShadowMapWidth = SCREEN_WIDTH / 16.0;
			m_iShadowMapHeight = SCREEN_HEIGHT / 16.0f;

			m_pData = new LightData();
			FillBasicProperties(color, pos, dir);
		}
		~Light();// { SAFE_DELETE(m_pData); }

		AVIRTUAL ABOOL VInitialize(INPUT_LAYOUT * pLayout);

		//Fill with data
		AVOID FillBasicProperties(const Vec& color, const Vec& pos, const Vec& dir)
		{
			m_pData->m_color = color;
			m_pData->m_pos = pos;
			m_pData->m_dir = dir;

			//m_pData->m_color = Vector(1.0f, 1.0f, 1.0f, 1.0f);
			//m_pData->m_pos = Vector(1.0f, 1.0f, 1.0f, 1.0f);
			//m_pData->m_dir = Vector(1.0f, 1.0f, 1.0f, 1.0f);
		}

		/**
			== Getters ==
							**/
		ABOOL IsInitialized() const { return m_bInitialized; }

		//Get color
		//Vec VGetColor() const { return m_ldCommonData.m_color; }

		//Get light type
		AVIRTUAL LightType VGetType() const = 0;

		/**
			== Rendering ==
							**/
		AVIRTUAL AVOID VPreRender(Renderer *pRenderer);
		AVIRTUAL AVOID VRender();
		AVIRTUAL AVOID VPostRender(Renderer* pRenderer);

		AVIRTUAL AVOID VPrepareToGenerateShadowMap(const Mat4x4 & world, Renderer * pRenderer);
		AVOID ClearShadowMap();
		AVIRTUAL AVOID VFinishShadowMapGeneration(Renderer *pRenderer);

		//for optimizations purposes only. generates screen-space rect
		//passed to the rasterizer for scissor test
		AVIRTUAL AVOID VSetScissorRect(Renderer* pRenderer, CameraPtr pCamera) = 0;

		/** 
			== Accessors ==
							**/
		ShaderResourceView* GetVarianceShadowMapSRV() const
		{
			return m_pVarianceShadowSRV;
		}
		UnorderedAccessView* GetVarianceShadowUAV() const
		{
			return m_pVarianceShadowUAV;
		}
		UnorderedAccessView* GetTempUAV() const
		{
			return m_pTempUAV;
		}
		ShaderResourceView* GetTempSRV() const
		{
			return m_pTempSRV;
		}

		//tramsforms
		AVOID SetDirection(const Vec & dir) { m_pData->m_dir = dir; }
		AVOID SetWorldTransform(const Mat4x4 & transform);
		AINLINE Mat4x4 GetWorldTransform() const { return m_worldTransform; }
	};

	typedef shared_ptr<Light>	LightPtr;
	typedef vector<LightPtr>	Lights;

}; //Anubis