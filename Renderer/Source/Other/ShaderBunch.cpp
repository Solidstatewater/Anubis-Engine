//====================================================================================
//ShaderBunch.cpp
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

#include "Renderer_pch.h"
#include "ShaderBunch.h"

using namespace Anubis;

ShaderBunch::~ShaderBunch()
{
	SAFE_DELETE(m_pPixelShader);
	SAFE_DELETE(m_pVertexShader);
}

AVOID ShaderBunch::VBind()
{
	m_pVertexShader->Set();
	m_pPixelShader->Set();
}

//Shaders
AVOID ShaderBunch::VSetVertexShader		(const AWSTRING & fileName, const ASTRING & shaderName, INPUT_LAYOUT* layout,
										 AUINT8 num, AUINT16 topology)
{
	//create shader
	//m_pVertexShader = make_shared<VertexShader>(VertexShader());
	m_pVertexShader = new VertexShader;

	//create and compile shader
	BlobDX11* pErrors = new BlobDX11();
	m_pVertexShader->CreateAndCompile(fileName, shaderName, layout, num, topology, pErrors);
}

AVOID ShaderBunch::VSetHullShader		(const AWSTRING & fileName, const ASTRING & shaderName)
{
	//empty here...
}

AVOID ShaderBunch::VSetDomainShader		(const AWSTRING & fileName, const ASTRING & shaderName)
{
	//empty here...
}

AVOID ShaderBunch::VSetGeometryShader	(const AWSTRING & fileName, const ASTRING & shaderName)
{
	//empty here...
}

AVOID ShaderBunch::VSetPixelShader		(const AWSTRING & fileName, const  ASTRING & shaderName)
{
	//initialize shader
	//PixelShader* pShader = new PixelShader();
	//m_pPixelShader = make_shared<PixelShader>(shader);
	//m_pPixelShader.reset(pShader);
	m_pPixelShader = new PixelShader();

	//create and compile shader
	BlobDX11* pErrors = new BlobDX11();
	m_pPixelShader->CreateAndCompile(fileName, shaderName, pErrors);
}