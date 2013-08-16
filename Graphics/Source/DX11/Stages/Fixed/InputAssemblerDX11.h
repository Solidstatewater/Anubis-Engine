#pragma once
//====================================================================================
//InputAssemblerDX11.h
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

#define INPUT_LAYOUT D3D11_INPUT_ELEMENT_DESC

namespace Anubis
{
	/*INPUT_LAYOUT InputLayout(	ASTRING SemanticName,
								UINT SemanticIndex,
								DXGI_FORMAT Format,
								UINT InputSlot,
								UINT AlignedByteOffset,
								D3D11_INPUT_CLASSIFICATION InputSlotClass,
  UINT                       InstanceDataStepRate) */

	class InputAssemblerStateDX11
	{
		friend class VertexShaderDX11;

	protected:
		ID3D11InputLayout*			m_pLayout;
		INPUT_LAYOUT*				m_pDesc;
		AUINT8						m_num;
		D3D11_PRIMITIVE_TOPOLOGY	m_topology;

	public:
		InputAssemblerStateDX11() {};
		~InputAssemblerStateDX11() { SAFE_DELETE_ARRAY(m_pDesc); }

		AVOID SetInputLayoutAndTopology() const;
	};
}; //Anubis