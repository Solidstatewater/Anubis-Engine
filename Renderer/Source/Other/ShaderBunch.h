#pragma once
//====================================================================================
//ShaderBunch.h
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

#include "Final\Stages\Shaders\Shaders.h"

namespace Anubis
{
	// =========================================
	//	Use this class to make pipeline bunches
	//	programmable stages
	// =========================================
	class ShaderBunch
	{
	private:
		//VertexShaderPtr	m_pVertexShader;
		//PixelShaderPtr	m_pPixelShader;
		VertexShader*	m_pVertexShader;
		PixelShader*	m_pPixelShader;

	public:
		//constructor
		ShaderBunch() {
			m_pVertexShader = nullptr;
			m_pPixelShader = nullptr;
		}

		//destrutor
		~ShaderBunch();

		//Shaders
		AVIRTUAL AVOID VSetVertexShader		(const AWSTRING & fileName, const ASTRING & shaderName, INPUT_LAYOUT* layout,
											 AUINT8 num, AUINT16 topology);
		AVIRTUAL AVOID VSetHullShader		(const AWSTRING & fileName, const ASTRING & shaderName);
		AVIRTUAL AVOID VSetDomainShader		(const AWSTRING & fileName, const ASTRING & shaderName);
		AVIRTUAL AVOID VSetGeometryShader	(const AWSTRING & fileName, const ASTRING & shaderName);
		AVIRTUAL AVOID VSetPixelShader		(const AWSTRING & fileName, const ASTRING & shaderName);

		AVIRTUAL AVOID VBind();
	}; 
}; //Anubis