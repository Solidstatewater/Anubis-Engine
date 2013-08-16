//====================================================================================
//BlobDX11.cpp
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
#include "BlobDX11.h"
#include <D3Dcompiler.h>

#include "Error_Handling/Assert.h"

using namespace Anubis;

ABOOL BlobDX11::CompileShader(const AWSTRING & fileName, const ASTRING & entrypoint,
									ShaderType type, BlobDX11 * pErrors) const
{
	//Check what type of shader we are compiling
	ASTRING target;
	switch (type)
	{
		case ST_Vertex:
			target = "vs_5_0";
			break;
		case ST_Hull:
			target = "hs_5_0";
			break;
		case ST_Domain:
			target = "ds_5_0";
			break;
		case ST_Geometry:
			target = "gs_5_0";
			break;
		case ST_Pixel:
			target = "ps_5_0";
			break;
		case ST_Compute:
			target = "cs_5_0";
			break;
	};

	//Compilation
	HRESULT hr = D3DX11CompileFromFile(fileName.c_str(), 0, 0, entrypoint.c_str(), target.c_str(), 0, 0, 0, 
		(ID3DBlob**)&m_pBlob, &pErrors->m_pBlob, 0);

	//If any errors, draw them to debug output
	if (hr != S_OK)
	{
		OutputDebugStringA((char*)pErrors->m_pBlob->GetBufferPointer());
		CheckHR(hr);
		return false;
	}

	return true;
}