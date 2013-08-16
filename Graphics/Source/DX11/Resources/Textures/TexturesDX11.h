#pragma once
//====================================================================================
//TexturesDX11.h
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

#include "../../../Base/Resources/Textures/TexturesBase.h"
#include "../Views/ShaderResourceViewDX11.h"
#include "../Views/UnorderedAccessViewDX11.h"
#include "../Views/RenderTargetViewDX11.h"
#include "../Views/DepthStencilViewDX11.h"

#include "../ResourceDX11.h"

namespace Anubis
{
	class TextureDX11 : public ResourceDX11
	{
	public:
		/****
			==========================================
				Shader Resource View Creation
			==========================================
													 ****/
		AVIRTUAL ABOOL CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const = 0;

		AVIRTUAL ABOOL CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const = 0;

		/****
			==========================================
				Unordered Access View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const = 0;

		AVIRTUAL ABOOL CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const = 0;

		/****
			==========================================
				Render Target View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const = 0;

		AVIRTUAL ABOOL CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const = 0;

		/****
			==========================================
				Depth Stencil View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const = 0;

		AVIRTUAL ABOOL CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const = 0;
	};

	class Texture1DDX11 : public TextureDX11
	{
	public :

		//constructor
		Texture1DDX11() : TextureDX11()
		{
			m_pTexture = NULL;
		}

		AVIRTUAL ABOOL Create(const D3D11_TEXTURE1D_DESC* pParams);

		//TextureDX11 Implementation

		/****
			==========================================
				Shader Resource View Creation
			==========================================
													 ****/
		AVIRTUAL ABOOL CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const;

		AVIRTUAL ABOOL CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const;

		/****
			==========================================
				Unordered Access View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const;

		AVIRTUAL ABOOL CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const;

		/****
			==========================================
				Render Target View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const;

		AVIRTUAL ABOOL CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const;

		/****
			==========================================
				Depth Stencil View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const;

		AVIRTUAL ABOOL CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const;

		//ResourceDX11 Implementation
		AVIRTUAL ID3D11Resource* GetResourcePointer() const { return m_pTexture; }

	protected:
		ID3D11Texture1D*	m_pTexture;
	};

	class Texture2DDX11 : public TextureDX11
	{
	public:
		//Constructor
		Texture2DDX11() : TextureDX11() 
		{
			m_pTexture = NULL;
		}

		AVIRTUAL ABOOL Create(const D3D11_TEXTURE2D_DESC* pParams);

		//TextureDX11 Implementation

		/****
			==========================================
				Shader Resource View Creation
			==========================================
													 ****/
		AVIRTUAL ABOOL CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const;

		AVIRTUAL ABOOL CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const;

		/****
			==========================================
				Unordered Access View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const;

		AVIRTUAL ABOOL CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const;

		/****
			==========================================
				Render Target View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const;

		AVIRTUAL ABOOL CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const;

		/****
			==========================================
				Depth Stencil View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const;

		AVIRTUAL ABOOL CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const;

		//ResourceDX11 Implementation
		AVIRTUAL ID3D11Resource* GetResourcePointer() const { return m_pTexture; }

	protected:
		ID3D11Texture2D*	m_pTexture;
	};

	class Texture3DDX11 : public TextureDX11
	{
	public:

		//constructor
		Texture3DDX11() : TextureDX11()
		{
			m_pTexture = NULL;
		}

		AVIRTUAL ABOOL Create(const D3D11_TEXTURE3D_DESC* pParams);

		//TextureDX11 Implementation

		/****
			==========================================
				Shader Resource View Creation
			==========================================
													 ****/
		AVIRTUAL ABOOL CreateShaderResourceView(ShaderResourceViewDX11 & view, 
			const ShaderResourceViewParamsDX11 * params)	const;

		AVIRTUAL ABOOL CreateShaderResourceView(ID3D11ShaderResourceView** ppView, 
			const ShaderResourceViewParamsDX11 * params)	const;

		/****
			==========================================
				Unordered Access View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateUnorderedAccessView(UnorderedAccessViewDX11 & view,
			const UnorderedAccessViewParamsDX11 * params)	const;

		AVIRTUAL ABOOL CreateUnorderedAccessView(ID3D11UnorderedAccessView** ppView,
			const UnorderedAccessViewParamsDX11 * params)	const;

		/****
			==========================================
				Render Target View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateRenderTargetView(RenderTargetViewDX11 & view, 
			const RenderTargetViewParamsDX11 * params)		const;

		AVIRTUAL ABOOL CreateRenderTargetView(ID3D11RenderTargetView** ppView, 
			const RenderTargetViewParamsDX11 * params)		const;

		/****
			==========================================
				Depth Stencil View Creation
			==========================================
													 ****/

		AVIRTUAL ABOOL CreateDepthStencilView(DepthStencilViewDX11 & view, 
			const DepthStencilViewParamsDX11 * params)		const;

		AVIRTUAL ABOOL CreateDepthStencilView(ID3D11DepthStencilView** ppView, 
			const DepthStencilViewParamsDX11 * params)		const;

		//ResourceDX11 Implementation
		AVIRTUAL ID3D11Resource* GetResourcePointer() const { return m_pTexture; }

	protected:
		ID3D11Texture3D*	m_pTexture;
	};

}; //Anubis