#pragma once
//====================================================================================
//Views.h
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

#include "../../../Graphics_pch.h"
#include "../../../DX11/Resources/Views/ShaderResourceViewDX11.h"
#include "../../../DX11/Resources/Views/UnorderedAccessViewDX11.h"
#include "../../../DX11/Resources/Views/RenderTargetViewDX11.h"
#include "../../../DX11/Resources/Views/DepthStencilViewDX11.h"

namespace Anubis
{
	//=======================================//
		//	Params structures
	//=======================================//
	class ShaderResourceViewParams : public
		#ifdef ADX11_API
			ShaderResourceViewParamsDX11
		#endif
	{
	};

	class UnorderedAccessViewParams : public
		#ifdef ADX11_API
			UnorderedAccessViewParamsDX11
		#endif
	{
	};

	class RenderTargetViewParams : public
		#ifdef ADX11_API
			RenderTargetViewParamsDX11
		#endif
	{
	};

	class DepthStencilViewParams : public
		#ifdef ADX11_API
			DepthStencilViewParamsDX11
		#endif
	{
	};

	//=======================================//
		//	Views
	//=======================================//

	class ShaderResourceView : public
		#ifdef ADX11_API
			ShaderResourceViewDX11
		#endif
	{
	public:
		#ifdef ADX11_API
			ShaderResourceView() : ShaderResourceViewDX11() {}
		#endif
	};

	class UnorderedAccessView : public
		#ifdef ADX11_API
			UnorderedAccessViewDX11
		#endif
	{
	public:
		#ifdef ADX11_API
			UnorderedAccessView() : UnorderedAccessViewDX11() {}
		#endif
	};

	class RenderTargetView : public
		#ifdef ADX11_API
			RenderTargetViewDX11
		#endif
	{
	public:
		#ifdef ADX11_API
			RenderTargetView(ID3D11RenderTargetView * const pView)
				: RenderTargetViewDX11(pView) {}
			RenderTargetView() : RenderTargetViewDX11() {}
		#endif 
	};

	class DepthStencilView : public
		#ifdef ADX11_API
			DepthStencilViewDX11
		#endif
	{
	public:
		#ifdef ADX11_API
			DepthStencilView() : DepthStencilViewDX11() {}
		#endif
	};

	//=======================================//
		//	View Lists
	//=======================================//
	class ShaderResourceViewList : public
		#ifdef ADX11_API
			ShaderResourceViewListDX11
		#endif
	{
		//typedef Parent class
		#ifdef ADX11_API
			typedef ShaderResourceViewListDX11 Parent;
		#endif
			
	public:
		/*** Constructor accepts number of views in the list ***/
		ShaderResourceViewList(AUINT8 numviews) :
			Parent(numviews) {}
	};

	class RenderTargetViewList : public
		#ifdef ADX11_API
			RenderTargetViewListDX11
		#endif
	{
		//typedef Parent class
		#ifdef ADX11_API
			typedef RenderTargetViewListDX11 Parent;
		#endif
			
	public:
		/*** Constructor accepts number of views in the list ***/
		RenderTargetViewList(AUINT8 numviews) :
			Parent(numviews) {}
	};

	//typedefs
	typedef shared_ptr<ShaderResourceView>	ShaderResourceViewPtr;
	typedef shared_ptr<UnorderedAccessView>	UnorderedAccessViewPtr;
	typedef shared_ptr<DepthStencilView>	DepthStencilViewPtr;
	typedef shared_ptr<RenderTargetView>	RenderTargetViewPtr;
}; //Anubis