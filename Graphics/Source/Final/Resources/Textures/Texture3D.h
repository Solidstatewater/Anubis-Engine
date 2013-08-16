#pragma once
//====================================================================================
//Texture3D.h
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

#include "../../../Graphics_pch.h"
//#include "../../../DX11/Resources/Textures/TexturesDX11.h"

namespace Anubis
{
	/*class Texture3DParams : public
		#ifdef ADX11_API
			D3D11_TEXTURE3D_DESC
		#endif
	{
	public:
		//constructor
		Texture2DParams() { 
			//just zero memory of the structure
			ZeroMemory(this, sizeof(Texture2DParams));  }

		ABOOL Init(AUINT32 width,
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
							ABOOL	cpuWrite);
	};

	class Texture2D : public
		#ifdef ADX11_API
			Texture2DDX11
		#endif
		{
		}; */
}; //Anubis