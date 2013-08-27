#pragma once
//====================================================================================
//SamplerState.h
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

#include "../../DX11/States/SamplerStateDX11.h"

namespace Anubis
{
	#ifdef ADX11_API
		class SamplerStateParams : public D3D11_SAMPLER_DESC
		{
		public:
			SamplerStateParams() { ZeroMemory(this, sizeof(SamplerStateParams)); }
			ABOOL Init(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU,
				D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW,
				AREAL32 MipLODBias, AUINT32 MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc,
				AREAL32* BorderColor = NULL, AREAL32 MinLOD = 0, AREAL32 MaxLOD = MAX_FLOAT32);
		};
	#endif

		class SamplerState : public
			#ifdef ADX11_API
				SamplerStateDX11
			#endif
		{
		public:
			SamplerState()
#ifdef ADX11_API
			:	SamplerStateDX11()
#endif
			{}
		};
}; //Anubis