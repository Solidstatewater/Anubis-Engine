#pragma once
//====================================================================================
//General.h : common methods
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

#include "Resources\Views\Views.h"
#include "Other\Viewport.h"

namespace Anubis
{
	/**
		// Global methods
	**/

	//*** Initialize graphics API ***//
	//------------------------------------------
	//-- 1: Handle to the window
	//-- 2: Window width
	//-- 3: Window height
	//------------------------------------------
	ABOOL InitializeGraphics(HWND hwnd, AUINT32 width, AUINT32 height);

	//*** Fill render target with color ***//
	//------------------------------------------
	//-- 1: Color to fill with in float[4] format
	//-- 2*: Render target to fill
	//------------------------------------------
	AVOID ClearRenderTargetView(const AREAL32 * color, RenderTargetView * pView);

	//*** Fill render target with color ***//
	//------------------------------------------
	//-- 1: Set to true if we need to clean depth
	//-- 2: Set to true if we need to clean stencil
	//-- 3: Value to fill depth buffer
	//-- 4: Value to fill stencil buffer
	//-- 5*: Depth stencil view to fill
	//------------------------------------------
	AVOID ClearDepthStencilView(ABOOL depth, ABOOL stencil, AREAL32 depthValue, 
		AUINT8 stencilValue, DepthStencilView * pView);

	//*** Unbind shader resource views from the pipeline ***/
	//----------------------------------------------------------
	//-- 1: Slot to unbind
	//-- 2: Number of views
	//-- 3: Shader type to unbind from
	//----------------------------------------------------------
	AVOID UnbindShaderResourceViews(AUINT16 slot, AUINT8 numviews, ShaderType shadertype);

	//*** Unbind unordered access views from the pipeline ***/
	//----------------------------------------------------------
	//-- 1: Slot to unbind
	//-- 2: Number of views
	//----------------------------------------------------------
	AVOID UnbindUnorderedAccessViews(AUINT16 slot, AUINT8 numviews);

	//*** Unbind render target views from the pipeline ***/
	//--------------------------------------------------------
	//-- 1: Number of views
	//--------------------------------------------------------
	AVOID UnbindRenderTargetViews(AUINT8 numviews);

	/**
		// Global variables
	**/
	static RenderTargetView*			g_pRenderTargetView		= NULL; //back buffer render target
	static DepthStencilView*			g_pDepthStencilView		= NULL; //back buffer depth stencil

	static Viewport*					g_pViewport				= NULL; //main viewport

	#ifdef ADX11_API
		static ID3D11Device*			g_d3d11Device			= NULL;	//directx device pointer
		static ID3D11DeviceContext*		g_d3d11DeviceContext	= NULL; //directx device context pointer
		static IDXGISwapChain*			g_SwapChain				= NULL; //directx swap chain pointer
	#endif

	/**
		//Global getters
	*/
	RenderTargetView * GetRenderTargetView();
	DepthStencilView * GetDepthStencilView();


	AVOID SetRenderTargetView();
	AVOID SetDepthStencilView(DepthStencilView * pDepthStencilView);

	#ifdef ADX11_API	//directX 11 specific
		ID3D11Device*			D3D11Device();
		ID3D11DeviceContext*	D3D11DeviceContext();
		ID3D11DeviceContext*	D3D11DevCon();
	#endif

	/**
		// Dispatch and drawing methods
	**/
	AVOID Dispatch(AUINT32 groupCountX, AUINT32 groupCountY, AUINT32 groupCountZ);
	AVOID Draw(AUINT32 vertexCount, AUINT32 startVertexLocation);
	AVOID DrawIndexed(AUINT32 indexCount, AUINT32 startIndexLocation, AINT32 baseVertexLocation);

	/**
		// Presenting Back Buffer
	**/
	AVOID PresentBackBuffer();

	/**
		// Clean up methods
	**/
	AVOID CleanUpGraphics();

}; //Anubis