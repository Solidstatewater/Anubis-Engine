#pragma once
//====================================================================================
//Plane.h : plane geometry
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

#include "../Vectors.h"
#include "../Math.h"

namespace Anubis
{
	class Plane
	{
	private:
		//Coefficients of the plane equation
		Vec	m_coeff;

	public:
		//Constructor
		Plane() { m_coeff = Vector(0.0f, 0.0f, 0.0f, 0.0f); }

		/**
			Methods	
					**/
		AVOID Normalize() //normalize
		{
			AREAL mag; //magnitude

			//get coefficients
			AREAL a = getx(m_coeff), b = gety(m_coeff), c = getz(m_coeff), d = getw(m_coeff);

			//calculate magnitude
			mag = SqrtR32(a*a + b*b + c*c);

			m_coeff = Vector(a / mag, b / mag, c / mag, d / mag);
		}

		//check whether point is inside the plane
		ABOOL Inside(const Vec & point) const;
		ABOOL Inside(const Vec & point, const AREAL32 radius) const;

		/**
			Initializers
							**/
		//!!!! normal faces away from you if you send in verts in counter clockwise order !!!!//
		AVOID InitFromPoints(const Vec & p1, const Vec & p2, const Vec & p3)
		{ 
			//calculate vectors
			Vec v21 = p2 - p1;
			Vec v31 = p3 - p1;

			//calculate normal
			Vec normal = cross(v21, v31);
			AREAL D = Dot(-normal, p1);

			//assign coefficients and normalize
			m_coeff = Vector(getx(normal), gety(normal), getz(normal), D);
			Normalize();
		}

		/**
				Getters
							**/
		AINLINE Vec Normal() const;
		AINLINE AREAL32 D()  const;

	};
}; //Anubis