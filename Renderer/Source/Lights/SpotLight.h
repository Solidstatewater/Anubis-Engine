#pragma once
//====================================================================================
//SpotLight.h
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

#include "Light.h"

namespace Anubis
{
	class SpotLight : public Light
	{
	protected:

		//define structure with light properties
		//struct SpotLightData : public Light::LightData 
		//{
		//AREAL32	m_r32Range;
			AREAL32 m_r32InnerAngleInRadians;
			AREAL32 m_r32OuterAngleInRadians;

		//	SpotLightData(AREAL32 r32InnerAngleInRadians, AREAL32 r32OuterAngleInRadians)
		//		: m_r32InnerAngleInRadians(r32InnerAngleInRadians), m_r32OuterAngleInRadians(r32OuterAngleInRadians)
		//	{
		//	}
		//};

		//SpotLightData m_ldData;

	public:

		//Constructor
		SpotLight  (const Vec& color, const Vec& pos, const Vec& dir, const AREAL r32Range,
					const AREAL32 r32InnerAngleInRadians, const AREAL32 r32OuterAngleInRadians)
					: Light(color, pos, Vector(dir.x, dir.y, dir.z, 2))
		{
			m_r32Range = r32Range;

			m_r32InnerAngleInRadians = r32InnerAngleInRadians;
			m_r32OuterAngleInRadians = r32OuterAngleInRadians;
		}

		AVOID VPreRender(Renderer *pRenderer);
		AVOID VPostRender(Renderer *pRenderer);
		AVOID VSetScissorRect(Renderer* pRenderer, CameraPtr pCamera);
		/**
			== Getters ==
							**/
		AVIRTUAL LightType VGetType()	const { return LT_Spot; }

		//Vec GetPosition()				const { return m_pos; }
		//Vec GetDirection()				const { return m_dir; }
		//AREAL32 GetInnerAngle()			const { return m_r32InnerAngleInRadians; }
		//AREAL32 GetOuterAngle()			const { return m_r32OuterAngleInRadians; }
	};

}; //Anubis