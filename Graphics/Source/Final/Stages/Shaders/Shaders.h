#pragma once
//====================================================================================
//Shaders.h
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

#include "../../../Graphics_pch.h"
#include "../../../DX11/Stages/Shaders/VertexShaderDX11.h"
#include "../../../DX11/Stages/Shaders/HullShaderDX11.h"
#include "../../../DX11/Stages/Shaders/DomainShaderDX11.h"
#include "../../../DX11/Stages/Shaders/GeometryShaderDX11.h"
#include "../../../DX11/Stages/Shaders/PixelShaderDX11.h"
#include "../../../DX11/Stages/Shaders/ComputeShaderDX11.h"

namespace Anubis
{
	class VertexShader : public
		#ifdef ADX11_API
			VertexShaderDX11
		#endif
	{
	};
	typedef shared_ptr<VertexShader> VertexShaderPtr;

	class HullShader : public
		#ifdef ADX11_API
			HullShaderDX11
		#endif
	{
	};
	typedef shared_ptr<HullShader> HullShaderPtr;

	class DomainShader : public
		#ifdef ADX11_API
			DomainShaderDX11
		#endif
	{
	};
	typedef shared_ptr<DomainShader> DomainShaderPtr;

	class GeometryShader : public
		#ifdef ADX11_API
			GeometryShaderDX11
		#endif
	{
	};
	typedef shared_ptr<GeometryShader> GeometryShaderPtr;

	class PixelShader : public
		#ifdef ADX11_API
			PixelShaderDX11
		#endif
	{
	};
	typedef shared_ptr<PixelShader> PixelShaderPtr;

	class ComputeShader : public
		#ifdef ADX11_API
			ComputeShaderDX11
		#endif
	{
	};
	typedef shared_ptr<ComputeShader> ComputeShaderPtr;
}; //Anubis