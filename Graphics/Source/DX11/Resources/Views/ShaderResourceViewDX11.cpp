//====================================================================================
//ShaderResourceViewDX11.cpp
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
#include "ShaderResourceViewDX11.h"

using namespace Anubis;

/******************************************
	//// ShaderResourceViewParamsDX11 ////
******************************************/
ABOOL ShaderResourceViewParamsDX11::InitForTexture2D(	AUINT8 format,
														AUINT16 miplevels, 
														AUINT16 mostdetailedmip	)
{
	Format = static_cast<DXGI_FORMAT>(format);
	Texture2D.MipLevels = 1;
	Texture2D.MostDetailedMip = 0;
	ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	return true;
}

ABOOL ShaderResourceViewDX11::CreateFromFile(LPCWSTR fileName)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D11Device(), fileName, nullptr, nullptr, &m_pView, nullptr);
	if (hr != S_OK)
	{
		CheckHR(hr);
		return false;
	}

	return true;
}

/******************************************
	//// ShaderResourceViewDX11 ////
******************************************/
AVOID ShaderResourceViewDX11::Set(AUINT16 slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Vertex:
		D3D11DeviceContext()->VSSetShaderResources(slot, 1, &m_pView);
		break;
	case ST_Domain:
		D3D11DeviceContext()->DSSetShaderResources(slot, 1, &m_pView);
		break;
	case ST_Geometry:
		D3D11DeviceContext()->GSSetShaderResources(slot, 1, &m_pView);
		break;
	case ST_Pixel:
		D3D11DeviceContext()->PSSetShaderResources(slot, 1, &m_pView);
		break;
	case ST_Compute:
		D3D11DeviceContext()->CSSetShaderResources(slot, 1, &m_pView);
	default:
		ASSERT(0);
		break;
	};
}

/******************************************
	//// ShaderResourceViewListDX11 ////
******************************************/
ShaderResourceViewListDX11::ShaderResourceViewListDX11(AUINT8 numViews)
	: m_numViews(numViews)
{
	//Initialze array of pointers to shader resource views
	m_ppViews = new ID3D11ShaderResourceView*[numViews];
}

ShaderResourceViewListDX11::~ShaderResourceViewListDX11()
{
	AUINT8 i;
	for (i = (m_numViews - 1); i > 0; i--)
		SAFE_RELEASE(m_ppViews[i]);

	SAFE_DELETE_ARRAY(m_ppViews);
}

ID3D11ShaderResourceView** ShaderResourceViewListDX11::GetView(AUINT8 index) const
{
	return &m_ppViews[index];
}

AVOID ShaderResourceViewListDX11::Set(AUINT16 slot, ShaderType type) const
{
	switch (type)
	{
	case ST_Vertex:
		D3D11DeviceContext()->VSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Domain:
		D3D11DeviceContext()->DSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Geometry:
		D3D11DeviceContext()->GSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	case ST_Pixel:
		D3D11DeviceContext()->PSSetShaderResources(slot, m_numViews, m_ppViews);
		break;
	default:
		ASSERT(0);
		break;
	};
}

AVOID ShaderResourceViewListDX11::SetView(AUINT8 index, AUINT16 slot, ShaderType type) const
{
	ID3D11ShaderResourceView* pSRV = m_ppViews[index];
	switch (type)
	{
	case ST_Vertex:
		D3D11DeviceContext()->VSSetShaderResources(slot, 1, &pSRV);
		break;
	case ST_Domain:
		D3D11DeviceContext()->DSSetShaderResources(slot, 1, &pSRV);
		break;
	case ST_Geometry:
		D3D11DeviceContext()->GSSetShaderResources(slot, 1, &pSRV);
		break;
	case ST_Pixel:
		D3D11DeviceContext()->PSSetShaderResources(slot, 1, &pSRV);
		break;
	default:
		ASSERT(0);
		break;
	};
}
