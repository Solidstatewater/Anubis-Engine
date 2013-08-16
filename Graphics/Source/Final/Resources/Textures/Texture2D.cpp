//====================================================================================
//Texture2D.cpp
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
#include "Texture2D.h"

using namespace Anubis;

ABOOL Texture2DParams::Init(AUINT32 width,
							AUINT32 height,
							AUINT16 arraySize,
							AUINT8  format,
							ABOOL	shaderResource,
							ABOOL	unorderedAccess,
							ABOOL	renderTarget,
							ABOOL   depthStencil,
							AUINT16 sampleCount,
							AUINT16 sampleQuality,
							AUINT16 miplevels,
							ABOOL	gpuWrite,
							ABOOL	cpuRead,
							ABOOL	cpuWrite,
							ABOOL	bGenerateMipMaps)
{
	ZeroMemory(this, sizeof(Texture2DParams));
#ifdef ADX11_API
	Width = width;
	Height = height;
	ArraySize = arraySize;
	Format = (DXGI_FORMAT)format;
	SampleDesc.Count = sampleCount;
	SampleDesc.Quality = sampleQuality;
	MipLevels = miplevels;

	//set level of accessebility by CPU and GPU
	if (cpuRead && cpuWrite && gpuWrite)
	{
		Usage = D3D11_USAGE_STAGING;
		CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}
	else if (gpuWrite && !cpuRead && !cpuWrite)
	{
		Usage = D3D11_USAGE_DEFAULT;
	}
	else if (cpuWrite && !cpuRead && !gpuWrite)
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
	}

	//Set resource binding types
	if (shaderResource)
		BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (renderTarget)
		BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (unorderedAccess)
		BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	if (depthStencil)
		BindFlags |= D3D11_BIND_DEPTH_STENCIL;

	if (bGenerateMipMaps)
		MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
#endif

	return true;
} 