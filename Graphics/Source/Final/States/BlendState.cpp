//====================================================================================
//BlendState.cpp
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

#include "Graphics_pch.h"
#include "BlendState.h"

using namespace Anubis;

ABOOL RenderTargetBlendParams::Init(ABOOL blendEnable, AUINT8 srcBlend, AUINT8 destBlend,
			AUINT8 blendOp, AUINT8 srcBlendAlpha, AUINT8 destBlendAlpha, AUINT8 blendOpAlpha,
			AUINT8 renderTargetWriteMask)
{
	BlendEnable				= blendEnable;
	SrcBlend				= (D3D11_BLEND)srcBlend;
	DestBlend				= (D3D11_BLEND)destBlend;
	BlendOp					= (D3D11_BLEND_OP)blendOp;
	SrcBlendAlpha			= (D3D11_BLEND)srcBlendAlpha;
	DestBlendAlpha			= (D3D11_BLEND)destBlendAlpha;
	BlendOpAlpha			= (D3D11_BLEND_OP)blendOpAlpha;
	RenderTargetWriteMask	= renderTargetWriteMask; 


	return true;
}

ABOOL BlendStateParams::Init(ABOOL alphaToCoverageEnable, ABOOL independentBlendEnable,
			RenderTargetBlendParams* targets, AUINT8 numParams)
{
	AlphaToCoverageEnable = alphaToCoverageEnable;
	IndependentBlendEnable = independentBlendEnable;
	
	for (int i = 0; i < numParams; i++)
		RenderTarget[i] = targets[i];

	if (numParams < 8 && !independentBlendEnable)
	{
		for (int i =0; i < 8; i++)
		{
			RenderTarget[i] = *targets;
		}
	} 

	/*IndependentBlendEnable = true;
	for (int i =0; i < 7; i++)
	{
	RenderTarget[i].BlendEnable = true;
	RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
	RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
	RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	RenderTarget[i].RenderTargetWriteMask = 0x0f;
	} */

	return true;
}