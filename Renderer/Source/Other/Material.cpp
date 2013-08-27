//====================================================================================
//Material.cpp
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
#include "Material.h"

#include "Final\General.h"

using namespace Anubis;

Material::Material() :	m_i32DiffuseSlot(-1), m_i32NormalSlot(-1),
						m_i32SpecularSlot(-1), m_i32HeightSlot(-1)
{
	m_pDiffuseTex = nullptr;
	m_pNormalTex = nullptr;
	m_pSpecularTex = nullptr;
	m_pHeightTex = nullptr;

	m_pDiffuseSRV	= new ShaderResourceView();
	m_pNormalSRV	= new ShaderResourceView();
	m_pSpecularSRV	= new ShaderResourceView();
	m_pHeightSRV	= new ShaderResourceView();;
}

Material::~Material()
{
	SAFE_DELETE(m_pHeightSRV);
	SAFE_DELETE(m_pSpecularSRV);
	SAFE_DELETE(m_pNormalSRV);
	SAFE_DELETE(m_pDiffuseSRV);

	SAFE_DELETE(m_pHeightTex);
	SAFE_DELETE(m_pSpecularTex);
	SAFE_DELETE(m_pNormalTex);
	SAFE_DELETE(m_pDiffuseTex);
}

ABOOL Material::VInitialize(AWSTRING diffuseFileName, AWSTRING normalFileName,
							AWSTRING specularFileName,AWSTRING heightFileName)
{
	if (!m_pDiffuseSRV->CreateFromFile(diffuseFileName.c_str()))
	{
		assert(0 && "Error loading material diffuse texture!");
		return false;
	}

	if (!m_pNormalSRV->CreateFromFile(normalFileName.c_str()))
	{
		assert(0 && "Error loading material normal map!");
		return false;
	}

	if (!m_pSpecularSRV->CreateFromFile(specularFileName.c_str()))
	{
		assert(0 && "Error loading material specular map!");
		return false;
	}

	if (!m_pHeightSRV->CreateFromFile(heightFileName.c_str()))
	{
		assert(0 && "Error loading material height map!");
		return false;
	}

	return true;
}

ABOOL Material::SetDiffuseTexture(AWSTRING diffuseFileName)
{
	if (!m_pDiffuseSRV->CreateFromFile(diffuseFileName.c_str()))
	{
		assert(0 && "Error loading material diffuse texture!");
		return false;
	}
}

ABOOL Material::SetNormalTexture(AWSTRING normalFileName)
{
	if (!m_pNormalSRV->CreateFromFile(normalFileName.c_str()))
	{
		assert(0 && "Error loading material normal map!");
		return false;
	}
}

ABOOL Material::SetSpecularTexture(AWSTRING specularFileName)
{
	if (!m_pSpecularSRV->CreateFromFile(specularFileName.c_str()))
	{
		assert(0 && "Error loading material specular map!");
		return false;
	}
}

ABOOL Material::SetHeightTexture(AWSTRING heightFileName)
{
	if (!m_pHeightSRV->CreateFromFile(heightFileName.c_str()))
	{
		assert(0 && "Error loading material height map!");
		return false;
	}
}

AVOID Material::BindDiffuse	(AUINT16 slot)
{
	m_pDiffuseSRV->Set(slot, PIXEL_SHADER);
	m_i32DiffuseSlot = slot;
}

AVOID Material::BindNormal	(AUINT16 slot)
{
	m_pNormalSRV->Set(slot, PIXEL_SHADER);
	m_i32NormalSlot = slot;
}

AVOID Material::BindSpecular	(AUINT16 slot)
{
	m_pSpecularSRV->Set(slot, PIXEL_SHADER);
	m_i32SpecularSlot = slot;
}

AVOID Material::BindHeight	(AUINT16 slot)
{
	m_pHeightSRV->Set(slot, PIXEL_SHADER);
	m_i32HeightSlot = slot;
}

AVOID Material::Set(AUINT16 slot)
{
	//don't use methods declared above to avoid 
	//method call overhead

	//bind diffuse
	m_pDiffuseSRV->Set(slot, PIXEL_SHADER);
	m_i32DiffuseSlot = slot++;

	//bind normal
	m_pNormalSRV->Set(slot, PIXEL_SHADER);
	m_i32NormalSlot = slot++;

	//bind specular
	m_pSpecularSRV->Set(slot, PIXEL_SHADER);
	m_i32SpecularSlot = slot++;

	//bind height
	m_pHeightSRV->Set(slot, PIXEL_SHADER);
	m_i32HeightSlot = slot;
}

AVOID Material::UnbindDiffuse	(const AUINT16 slot)
{
	UnbindShaderResourceViews(m_i32DiffuseSlot, 1, PIXEL_SHADER);
	m_i32DiffuseSlot = -1;
}

AVOID Material::UnbindNormal	(const AUINT16 slot)
{
	UnbindShaderResourceViews(m_i32NormalSlot, 1, PIXEL_SHADER);
	m_i32NormalSlot = -1;
}

AVOID Material::UnbindSpecular	(const AUINT16 slot)
{
	UnbindShaderResourceViews(m_i32SpecularSlot, 1, PIXEL_SHADER);
	m_i32SpecularSlot = -1;
}

AVOID Material::UnbindHeight	(const AUINT16 slot)
{
	UnbindShaderResourceViews(m_i32HeightSlot, 1, PIXEL_SHADER);
	m_i32HeightSlot = -1;
}

AVOID Material::Unbind()
{
	UnbindShaderResourceViews(m_i32DiffuseSlot, 4, PIXEL_SHADER);
	m_i32DiffuseSlot = m_i32NormalSlot = m_i32SpecularSlot = m_i32HeightSlot = -1;
}