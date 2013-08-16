#pragma once
//====================================================================================
//RenderTargetViewDX11.h
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

#include "DepthStencilViewDX11.h"

namespace Anubis
{
	class RenderTargetViewParamsDX11 : public D3D11_RENDER_TARGET_VIEW_DESC
	{
	public:
		RenderTargetViewParamsDX11() { ZeroMemory(this, sizeof(RenderTargetViewParamsDX11)); }
		ABOOL InitForTexture2D( AUINT8 format,
								AUINT16 mipslice);
	};

	class RenderTargetViewDX11
	{
	public:
		ID3D11RenderTargetView*	m_pView;

	public:
		RenderTargetViewDX11() { m_pView = NULL; }
		RenderTargetViewDX11(ID3D11RenderTargetView * const pView) { m_pView = pView; }

		~RenderTargetViewDX11() { SAFE_RELEASE(m_pView); }

		AVOID Set(const DepthStencilViewDX11 & depthview) const;
		AVOID Set() const;
		AVOID Clear(const AREAL * bgColor);
		AVOID Clear();
	};

	class RenderTargetViewListDX11
	{
		ID3D11RenderTargetView**	m_ppViews;
		AUINT8	m_numViews;

	public:
		/*  ==
			Constructor and Destructor
			                          == */
		RenderTargetViewListDX11(AUINT8 numViews);
		~RenderTargetViewListDX11();

		/*** Getters ***/
		ID3D11RenderTargetView** GetView(AUINT8 index) const;

		/* ==
			Bind Shader Resource Views to the pipeline
		== */
		AVOID Set(DepthStencilViewDX11 * pView) const;
		AVOID Clear(const AREAL * bgColor);
		AVOID Clear();
	};
}; //Anubis