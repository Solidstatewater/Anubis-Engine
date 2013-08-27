//====================================================================================
//RasterizerState.cpp
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
#include "RasterizerState.h"

using namespace Anubis;

ABOOL RasterizerParams::Init(const ABOOL wireframe, const ABOOL cullFront, const ABOOL cullBack, const ABOOL frontCounterClockwise, const AINT32 depthBias,
				const AREAL depthBiasClamp, const AREAL slopeScaledDepthBias, const ABOOL depthClipEnable, const ABOOL scissorEnable, const ABOOL multiSampleEnable,
				const ABOOL antialiasedLineEnable)
{
	if (wireframe)
		FillMode = D3D11_FILL_WIREFRAME;
	else
		FillMode = D3D11_FILL_SOLID;

	if (!cullFront && !cullBack)
		CullMode = D3D11_CULL_NONE;
	else if (cullFront && !cullBack)
		CullMode = D3D11_CULL_FRONT;
	else if (!cullFront && cullBack)
		CullMode = D3D11_CULL_BACK;
	else if (cullFront && cullBack)
		assert(0 && "Rasterizer cannot cull front and back faces concurrently!");

	FrontCounterClockwise = frontCounterClockwise;
	DepthBias = depthBias;
	DepthBiasClamp = depthBiasClamp;
	SlopeScaledDepthBias = slopeScaledDepthBias;
	DepthClipEnable = depthClipEnable;
	ScissorEnable = scissorEnable;
	MultisampleEnable = multiSampleEnable;
	AntialiasedLineEnable = antialiasedLineEnable;

	return true;
}