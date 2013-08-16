//====================================================================================
//TexturesDX11.cpp
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
#include "TexturesDX11.h"


using namespace Anubis;

////////////////////////////////////////
//Texture1D Implementation
////////////////////////////////////////
ABOOL Texture1DDX11::CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture1DDX11::CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture1DDX11::CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture1DDX11::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture1DDX11::CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, &view.m_pView);
	 
	VALID(hr);
}

ABOOL Texture1DDX11::CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, ppView);
	 
	VALID(hr);
}

ABOOL Texture1DDX11::CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture1DDX11::CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture1DDX11::Create(const D3D11_TEXTURE1D_DESC* pParams)
{
	HRESULT hr = D3D11Device()->CreateTexture1D(pParams, NULL, &m_pTexture);
	
	VALID(hr);
}

////////////////////////////////////////
//Texture2D Implementation
////////////////////////////////////////
ABOOL Anubis::Texture2DDX11::CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}
ABOOL Anubis::Texture2DDX11::CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, ppView);

	VALID(hr);
}


ABOOL Texture2DDX11::CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}
ABOOL Texture2DDX11::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, ppView);

	VALID(hr);
}


ABOOL Texture2DDX11::CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, &view.m_pView);
	 
	VALID(hr);
}

ABOOL Texture2DDX11::CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, ppView);
	 
	VALID(hr);
}

ABOOL Texture2DDX11::CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture2DDX11::CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture2DDX11::Create(const D3D11_TEXTURE2D_DESC* pParams)
{
	HRESULT hr = D3D11Device()->CreateTexture2D(pParams, NULL, &m_pTexture);
	
	VALID(hr);
}

////////////////////////////////////////
//Texture3D Implementation
////////////////////////////////////////
ABOOL Texture3DDX11::CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture3DDX11::CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateShaderResourceView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture3DDX11::CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture3DDX11::CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const
{
	HRESULT hr = D3D11Device()->CreateUnorderedAccessView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture3DDX11::CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, &view.m_pView);
	 
	VALID(hr);
}

ABOOL Texture3DDX11::CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateRenderTargetView(m_pTexture, params, ppView);
	 
	VALID(hr);
}

ABOOL Texture3DDX11::CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, &view.m_pView);

	VALID(hr);
}

ABOOL Texture3DDX11::CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const
{
	HRESULT hr = D3D11Device()->CreateDepthStencilView(m_pTexture, params, ppView);

	VALID(hr);
}

ABOOL Texture3DDX11::Create(const D3D11_TEXTURE3D_DESC* pParams)
{
	HRESULT hr = D3D11Device()->CreateTexture3D(pParams, NULL, &m_pTexture);
	
	VALID(hr);
}