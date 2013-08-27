#pragma once
//====================================================================================
//GBuffer.h
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

//#include "Final/Resources/Textures/Texture1D.h"
#include "Final/Resources/Textures/Texture2D.h"
#include "Final/Resources/Textures/Texture3D.h"
#include "Final/Resources/Views/Views.h"

namespace Anubis
{
	struct GBuffer
	{
		/***
			Initialize buffer
		***/
		AVIRTUAL ABOOL VInitialize() = 0;

		/***
			Bind GBuffer for reading and writing
		***/
		AVIRTUAL AVOID BindForWriting(DepthStencilView * pView) = 0;
		AVIRTUAL AVOID BindForReading(AUINT16 slot) const = 0;

		AVIRTUAL AVOID BindPositionTex(AUINT16 slot, ShaderType shaderType) const = 0;
		AVIRTUAL AVOID BindNormalTex(AUINT16 slot, ShaderType shaderType) const = 0;
		AVIRTUAL AVOID BindDepthTex(AUINT16 slot, ShaderType shaderType) const = 0;
		AVIRTUAL AVOID BindDiffuseTex(AUINT16 slot, ShaderType shaderType) const  = 0;
		AVIRTUAL AVOID BindSpecularTex(AUINT16 slot, ShaderType shaderType) const = 0;

		/***
			Unbind buffer resources from the pipeline
		***/
		AVIRTUAL VOID UnbindFromWriting() const = 0;
		AVIRTUAL VOID UnbindFromReading(AUINT16 slot) const = 0;

		/***
			Clear g-buffer after rendering
		***/
		AVIRTUAL VOID Clear() = 0;
	};

	struct GBufferOne : public GBuffer
	{
		/***
			== Textures
						***/
		Texture2DPtr	m_pPosTex;
		Texture2DPtr	m_pNormalTex;
		Texture2DPtr	m_pDepthTex;
		Texture2DPtr	m_pDiffuseTex;
		Texture2DPtr	m_pSpecularTex;
		Texture2DPtr	m_pGeometryNormalTex;

		/***
			== ShaderResource View
									***/
		/* ShaderResourceView &	m_pPosSRV;
		ShaderResourceView &	m_pNormalSRV;
		ShaderResourceView &	m_pDiffuseSRV;
		ShaderResourceView &	m_pSpecularSRV; */
		ShaderResourceViewList	m_SRVList;

		/***
			== UnorderedAccess View
									***/
		/*RenderTargetView &	m_pPosRTV;
		RenderTargetView &	m_pNormalRTV;
		RenderTargetView &	m_pDiffuseRTV;
		RenderTargetView &	m_pSpecularRTV; */
		RenderTargetViewList	m_RTVList;

	public:

		/***
			Constructor, Destructor
		***/
		GBufferOne();
		~GBufferOne()	{}

		/***
			Initilize
		***/
		AVIRTUAL ABOOL VInitialize();

		/***
			Bind GBuffer for reading and writing
		***/
		AVIRTUAL AVOID BindForWriting(DepthStencilView * pView);
		AVIRTUAL AVOID BindForReading(AUINT16 slot) const;

		AVIRTUAL AVOID BindPositionTex(AUINT16 slot, ShaderType shaderType) const;
		AVIRTUAL AVOID BindNormalTex(AUINT16 slot, ShaderType shaderType) const;
		AVIRTUAL AVOID BindDepthTex(AUINT16 slot, ShaderType shaderType) const;
		AVIRTUAL AVOID BindDiffuseTex(AUINT16 slot, ShaderType shaderType) const;
		AVIRTUAL AVOID BindSpecularTex(AUINT16 slot, ShaderType shaderType) const;

		/***
			Unbind buffer resources from the pipeline
		***/
		AVIRTUAL AVOID UnbindFromWriting() const;
		AVIRTUAL AVOID UnbindFromReading(AUINT16 slot) const;

		/***
			Clear buffer after rendering
		***/
		AVIRTUAL AVOID Clear();
	};
};