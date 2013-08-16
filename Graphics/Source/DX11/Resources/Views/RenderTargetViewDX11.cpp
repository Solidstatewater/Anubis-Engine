//====================================================================================
//RenderTargetViewDX11.cpp
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

#include "Graphics_pch.h"
#include "../../../Final/General.h"
#include "RenderTargetViewDX11.h"

using namespace Anubis;

/** ==
	RenderTargetViewParamsDX11
== **/
ABOOL RenderTargetViewParamsDX11::InitForTexture2D(	AUINT8 format,
													AUINT16 mipslice)
{
	Format = static_cast<DXGI_FORMAT>(format);
	Texture2D.MipSlice = mipslice;
	ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	return true;
}

/** ==
	RenderTargetViewDX11
== **/
AVOID RenderTargetViewDX11::Set(const DepthStencilViewDX11 & depthview) const
{
	D3D11DeviceContext()->OMSetRenderTargets(1, &m_pView, depthview.m_pView);
}

AVOID RenderTargetViewDX11::Set() const
{
	D3D11DeviceContext()->OMSetRenderTargets(1, &m_pView, NULL);
}

AVOID RenderTargetViewDX11::Clear(const AREAL * bgColor)
{
	D3D11DeviceContext()->ClearRenderTargetView(m_pView, bgColor);
}

AVOID RenderTargetViewDX11::Clear()
{
	const AREAL color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	D3D11DeviceContext()->ClearRenderTargetView(m_pView, color);
}

/** ==
	RenderTargetViewListDX11
== **/
RenderTargetViewListDX11::RenderTargetViewListDX11(AUINT8 numViews)
	: m_numViews(numViews)
{
	//Initialze array of pointers to shader resource views
	m_ppViews = new ID3D11RenderTargetView*[numViews];
}

RenderTargetViewListDX11::~RenderTargetViewListDX11()
{
	AUINT8 i;
	for (i = (m_numViews - 1); i > 0; i--)
		SAFE_RELEASE(m_ppViews[i]);

	SAFE_DELETE_ARRAY(m_ppViews);
}

ID3D11RenderTargetView** RenderTargetViewListDX11::GetView(AUINT8 index) const
{
	return &m_ppViews[index];
}

AVOID RenderTargetViewListDX11::Set(DepthStencilViewDX11 * pView) const
{
	if (pView)
		D3D11DeviceContext()->OMSetRenderTargets(m_numViews, m_ppViews, pView->m_pView);
	else
		D3D11DeviceContext()->OMSetRenderTargets(m_numViews, m_ppViews, nullptr);
}

AVOID RenderTargetViewListDX11::Clear(const AREAL * bgColor)
{
	AUINT8 i;
	for (i = 0; i < m_numViews; i++)
		D3D11DeviceContext()->ClearRenderTargetView(m_ppViews[i], bgColor);
}

AVOID RenderTargetViewListDX11::Clear()
{
	const AREAL color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	AUINT8 i;

	for (i = 0; i < m_numViews; i++)
		D3D11DeviceContext()->ClearRenderTargetView(m_ppViews[i], color);
}