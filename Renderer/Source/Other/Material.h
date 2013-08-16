#pragma once
//====================================================================================
//Material.h
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

#include "Final\Resources\Textures\Texture2D.h"
#include "Final\Resources\Views\Views.h"

namespace Anubis
{
	typedef AUINT32 MaterialId;

	class Material
	{
	protected:

		//unique id
		MaterialId	m_id;

		/*** ======================
				Textures
			 ====================== ***/

		Texture2D * m_pDiffuseTex;
		Texture2D * m_pNormalTex;
		Texture2D * m_pSpecularTex;
		Texture2D * m_pHeightTex;

		/*** =========================
				Shader Resource Views
		     ========================= ***/

		ShaderResourceView * m_pDiffuseSRV;
		ShaderResourceView * m_pNormalSRV;
		ShaderResourceView * m_pSpecularSRV;
		ShaderResourceView * m_pHeightSRV;

		// ===== Specific rendering data ===== //
		
		//remember the slot to which we bound resource view
		AINT32	m_i32DiffuseSlot;
		AINT32	m_i32NormalSlot;
		AINT32	m_i32SpecularSlot;
		AINT32	m_i32HeightSlot;

	public:
		Material();
		~Material();

		AVIRTUAL ABOOL VInitialize(AWSTRING diffuseFileName, AWSTRING normalFileName, AWSTRING specularFileName,
			AWSTRING heightFileName);

		ABOOL SetDiffuseTexture	(AWSTRING diffuseFileName);
		ABOOL SetNormalTexture	(AWSTRING normalFileName);
		ABOOL SetSpecularTexture(AWSTRING specularFileName);
		ABOOL SetHeightTexture	(AWSTRING heightFileName);

		// ===== Bind material to the pipeline ===== //

		AVOID BindDiffuse	(const AUINT16 slot);
		AVOID BindNormal	(const AUINT16 slot);
		AVOID BindSpecular	(const AUINT16 slot);
		AVOID BindHeight	(const AUINT16 slot);

		AVOID Set(const AUINT16 slot); //always bind to the pixel shader

		// ===== Unbind material form the pipeline ===== //

		AVOID UnbindDiffuse	(const AUINT16 slot);
		AVOID UnbindNormal	(const AUINT16 slot);
		AVOID UnbindSpecular(const AUINT16 slot);
		AVOID UnbindHeight	(const AUINT16 slot);

		AVOID Unbind(); //always unbind from the pixel shader
	};

	typedef std::shared_ptr<Material> MaterialPtr;
}; //Anubis