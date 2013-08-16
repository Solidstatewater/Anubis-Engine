//====================================================================================
//General.cpp : Implement common graphics methods
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
#include "General.h"

#include "../../../DXUT/DXUT.h"
#include "../../../DXUT/DXUTDevice11.h"

using namespace Anubis;

/**
	// Global methods Implementation
**/
ABOOL Anubis::InitializeGraphics(HWND hwnd, AUINT32 width, AUINT32 height)
{
#ifdef ADX11_API

	//=============================
	// Describe back buffer structure
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC)); //zero memory for the structure

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//bufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//=============================
	//Describe SwapChain structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC)); //zero memory for the structure

	swapChainDesc.BufferDesc = bufferDesc; //set created back buffer
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //use back buffer as render target
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd; 
	swapChainDesc.Windowed = TRUE; 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//=============================
	//Create swap chain
	D3D_FEATURE_LEVEL pLevel = D3D_FEATURE_LEVEL_11_0; //set feature desired feature level
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &pLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &g_SwapChain, &g_d3d11Device, NULL, &g_d3d11DeviceContext);

	//Get BackBuffer texture
	ID3D11Texture2D* BackBuffer;
	hr = g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&BackBuffer );

	//Create Render Target
	ID3D11RenderTargetView* pRenderTargetView;
	hr = g_d3d11Device->CreateRenderTargetView( BackBuffer, NULL, &pRenderTargetView );
	BackBuffer->Release();

	//Initialize global render target pointer
	g_pRenderTargetView = new RenderTargetView(pRenderTargetView);

	//Set Render Target
	g_d3d11DeviceContext->OMSetRenderTargets( 1, &pRenderTargetView, NULL );

	//Initialize and set viewport
	g_pViewport = new Viewport();
	g_pViewport->Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
	g_pViewport->Set();

	return true;
#endif //if defined ADX11_API
}

AVOID Anubis::ClearRenderTargetView(const AREAL32 * color, RenderTargetView * pView)
{
	if (!pView)
		D3D11DeviceContext()->ClearRenderTargetView(GetRenderTargetView()->m_pView, color);
	else
		D3D11DeviceContext()->ClearRenderTargetView(pView->m_pView, color);
}

AVOID Anubis::ClearDepthStencilView(ABOOL depth, ABOOL stencil, AREAL32 depthValue, 
		AUINT8 stencilValue, DepthStencilView * pView)
{
#ifdef ADX11_API
	//ID3D11DepthStencilView* pDSView;
	//if (!pView)
	//	pDSView = GetDepthStencilView()->m_pView;
	//else
	//	pDSView = pView->m_pView;
	if (!pView) return;

	if (depth && stencil)
		D3D11DeviceContext()->ClearDepthStencilView(pView->m_pView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depthValue, stencilValue);
	else if (depth && !stencil)
		D3D11DeviceContext()->ClearDepthStencilView(pView->m_pView, D3D11_CLEAR_DEPTH, depthValue, stencilValue);
	else if (!depth && stencil)
		D3D11DeviceContext()->ClearDepthStencilView(pView->m_pView, D3D11_CLEAR_STENCIL, depthValue, stencilValue);
#endif
}

AVOID Anubis::UnbindShaderResourceViews(AUINT16 slot, AUINT8 numviews, ShaderType shadertype)
{
#ifdef ADX11_API
	ID3D11ShaderResourceView* pNullSRV = NULL;

	switch (shadertype)
	{
	case ST_Vertex:
		for (int i = 0; i < numviews; i++)
			D3D11DeviceContext()->VSSetShaderResources(slot+i, 1, &pNullSRV);
		break;
	case ST_Domain:
		for (int i = 0; i < numviews; i++)
			D3D11DeviceContext()->DSSetShaderResources(slot+i, 1, &pNullSRV);
		break;
	case ST_Geometry:
		for (int i = 0; i < numviews; i++)
			D3D11DeviceContext()->GSSetShaderResources(slot+i, 1, &pNullSRV);
		break;
	case ST_Pixel:
		for (int i = 0; i < numviews; i++)
			D3D11DeviceContext()->PSSetShaderResources(slot+i, 1, &pNullSRV);
		break;
	case ST_Compute:
		for (int i = 0; i < numviews; i++)
			D3D11DeviceContext()->CSSetShaderResources(slot+i, 1, &pNullSRV);
	default:
		ASSERT(0);
		break;
	};
#endif
}

AVOID Anubis::UnbindUnorderedAccessViews(AUINT16 slot, AUINT8 numviews)
{
#ifdef ADX11_API
	ID3D11UnorderedAccessView* pNullUAV = NULL;

	D3D11DeviceContext()->CSSetUnorderedAccessViews(slot, numviews, &pNullUAV, nullptr);
#endif
}

AVOID Anubis::UnbindRenderTargetViews(AUINT8 numviews)
{
#ifdef ADX11_API
	ID3D11RenderTargetView* pNullRTV = NULL;
	for (int i = 0; i < numviews; i++)
		D3D11DeviceContext()->OMSetRenderTargets(1, &pNullRTV, NULL);
#endif
}

#ifdef ADX11_API
/**
	//Global getters Implementation
**/
ID3D11Device* Anubis::D3D11Device()
{
#ifdef DXUT11_FW
	return DXUTGetD3D11Device();
#else
	return g_d3d11Device;
#endif
}

ID3D11DeviceContext* Anubis::D3D11DeviceContext()
{
#ifdef DXUT11_FW
	return DXUTGetD3D11DeviceContext();
#else
	return g_d3d11DeviceContext;
#endif
}

ID3D11DeviceContext* Anubis::D3D11DevCon()
{
#ifdef DXUT11_FW
	return DXUTGetD3D11DeviceContext();
#else
	return g_d3d11DeviceContext;
#endif
}

#endif

/**
	//Global getters implementation
**/
RenderTargetView * Anubis::GetRenderTargetView()
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		return DXUTGetD3D11RenderTargetView();
	#else
		return g_pRenderTargetView;
	#endif
#endif
}

DepthStencilView * Anubis::GetDepthStencilView()
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		return DXUTGetD3D11DepthStencilView();
	#else
		return g_pDepthStencilView;
	#endif
#endif
}

AVOID Anubis::SetRenderTargetView()
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		DXUTGetD3D11DeviceContext()->OMSetRenderTargets( 1, DXUTGetD3D11RenderTargetView(), NULL );
	#else
		g_d3d11DeviceContext->OMSetRenderTargets( 1, &g_pRenderTargetView->m_pView, NULL );
	#endif
#endif
}

AVOID Anubis::SetDepthStencilView(DepthStencilView* pDSV)
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		DXUTGetD3D11DeviceContext()->OMSetRenderTargets(0, nullptr, pDSV->m_pView);
	#else
		g_d3d11DeviceContext->OMSetRenderTargets(0, nullptr, pDSV->m_pView);
	#endif
#endif
}

/**
	//Dispatch and drawing methods implementation
**/
AVOID Anubis::Dispatch(AUINT32 groupCountX, AUINT32 groupCountY, AUINT32 groupCountZ)
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		D3D11DeviceContext()->Dispatch(groupCountX, groupCountY, groupCountZ);
	#else
		g_d3d11DeviceContext->Dispatch(groupCountX, groupCountY, groupCountZ);
	#endif
#endif
}

AVOID Anubis::Draw(AUINT32 vertexCount, AUINT32 startVertexLocation)
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		D3D11DeviceContext()->Draw(vertexCount, startVertexLocation);
	#else
		g_d3d11DeviceContext->Draw(vertexCount, startVertexLocation);
	#endif
#endif
}

AVOID Anubis::DrawIndexed(AUINT32 indexCount, AUINT32 startIndexLocation, AINT32 baseVertexLocation)
{
#ifdef ADX11_API
	#ifdef DXUT11_FW
		D3D11DeviceContext()->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	#else
		g_d3d11DeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	#endif
#endif
}

/**
	// Presenting Back Buffer
**/
AVOID Anubis::PresentBackBuffer()
{
	g_SwapChain->Present(0, 0);
}

/**
		// Clean up methods
**/
AVOID Anubis::CleanUpGraphics()
{
#ifdef ADX11_API
	SAFE_RELEASE(g_d3d11Device);	
	SAFE_RELEASE(g_d3d11DeviceContext);
	SAFE_RELEASE(g_SwapChain);
#endif
}