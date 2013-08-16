//====================================================================================
//Math.cpp
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

#include "Math_pch.h"
#include "Math.h"

using namespace Anubis;

/*************************************
			Trigonometry
*************************************/

/**************
	== IMPORTANT~ ==
	Later on should be replaced with self implemented Tailor series calculations...
	==
***************/

AREAL32 Anubis::CosR32( AREAL32 angleInRadians ) { return cosf(angleInRadians); }
AREAL64 Anubis::CosR64( AREAL64 angleInRadians ) { return cos(angleInRadians); }
AREAL64 Anubis::Cos	  ( AREAL64 angleInRadians ) { return cos(angleInRadians); }

AREAL32 Anubis::SinR32( AREAL32 angleInRadians ) { return sinf(angleInRadians); }
AREAL64 Anubis::SinR64( AREAL64 angleInRadians ) { return sin(angleInRadians); }
AREAL64 Anubis::Sin	  ( AREAL64 angleInRadians ) { return sin(angleInRadians); }

AREAL32 Anubis::TgR32 ( AREAL32 angleInRadians ) { return tanf(angleInRadians); }
AREAL64 Anubis::TgR64 ( AREAL64 angleInRadians ) { return tan(angleInRadians); }
AREAL64 Anubis::Tg	  ( AREAL64 angleInRadians ) { return tan(angleInRadians); }

AREAL32 Anubis::CtgR32( AREAL32 angleInRadians ) { return atanf(angleInRadians); }
AREAL64 Anubis::CtgR64( AREAL64 angleInRadians ) { return atan(angleInRadians); }
AREAL64 Anubis::Ctg	  ( AREAL64 angleInRadians ) { return atan(angleInRadians); }

AREAL32 Anubis::Atan2R32( AREAL32 y, AREAL32 x ) { return atan2f(y, x); }
//AREAL64 Anubis::Atan2R64( AREAL64 y, AREAL32 x ) { return atan2((AREAL64)y, (AREAL32)x); }
AREAL64 Anubis::Atan2	( AREAL64 y, AREAL64 x ) { return atan2(y, x); }

AREAL32 Anubis::SqrtR32 (AREAL32 x) { return sqrtf(x); }
AREAL64 Anubis::SqrtR64 (AREAL64 x) { return sqrt(x); }
AREAL64 Anubis::Sqrt	(AREAL64 x) { return sqrt(x); }

/* ======================================
					Vectors
	========================================= */
AREAL Anubis::Dot(const Vec& v1, const Vec& v2)
{
	#ifdef SIMD_MATH_ENABLED
		Vec res = _mm_mul_ps(v1, v2);
		Vec x = _mm_replicate_x_ps(res);
		Vec y = _mm_replicate_y_ps(res);
		Vec z = _mm_replicate_z_ps(res);
		Vec w = _mm_replicate_w_ps(res);

		return getx(_mm_add_ps(w, _mm_add_ps(_mm_add_ps(z, y), x)));
	#else
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	#endif
}

AREAL Anubis::Length(const Vec & v)
{
	#ifdef SIMD_MATH_ENABLED
#else
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
#endif
}

Vec Anubis::Normalize(const Vec& V)
{
#ifdef SIMD_MATH_ENABLED
	// Perform the dot product on x,y and z only
    Vec vLengthSq = _mm_mul_ps(V,V);
    Vec vTemp = _mm_shuffle_ps(vLengthSq,vLengthSq,_MM_SHUFFLE(2,1,2,1));
    vLengthSq = _mm_add_ss(vLengthSq,vTemp);
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,1,1,1));
    vLengthSq = _mm_add_ss(vLengthSq,vTemp);
	vLengthSq = _mm_shuffle_ps(vLengthSq,vLengthSq,_MM_SHUFFLE(0,0,0,0));
    // Prepare for the division
    Vec vResult = _mm_sqrt_ps(vLengthSq);
    // Create zero with a single instruction
    Vec vZeroMask = _mm_setzero_ps();
    // Test for a divide by zero (Must be FP to detect -0.0)
    vZeroMask = _mm_cmpneq_ps(vZeroMask,vResult);
    // Failsafe on zero (Or epsilon) length planes
    // If the length is infinity, set the elements to zero
    vLengthSq = _mm_cmpneq_ps(vLengthSq,Vector(0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000));
    // Divide to perform the normalization
    vResult = _mm_div_ps(V,vResult);
    // Any that are infinity, set to zero
    vResult = _mm_and_ps(vResult,vZeroMask);
    // Select qnan or result based on infinite length
	Vec vTemp1 = _mm_andnot_ps(vLengthSq,Vector(0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000));
    Vec vTemp2 = _mm_and_ps(vResult,vLengthSq);
    vResult = _mm_or_ps(vTemp1,vTemp2);
    return vResult;
#else
	return V / Length(V);
#endif
}

/* ======================================
					Matrices
	========================================= */
Mat4x4 Anubis::CreateViewMatrixLH(const Vec & pos, const Vec & lookDir, const Vec & up)
{
	Mat4x4 mat;

	Vec & zaxis = Normalize(lookDir);
	Vec & xaxis = Normalize(cross(up, zaxis));
	Vec & yaxis = cross(zaxis, xaxis);

	mat.Init(	Vector( getx(xaxis), getx(yaxis), getx(zaxis), 0),
				Vector( gety(xaxis), gety(yaxis), gety(zaxis), 0),
				Vector( getz(xaxis), getz(yaxis), getz(zaxis), 0),
				Vector( -Dot(xaxis, pos), -Dot(yaxis, pos), -Dot(zaxis, pos), 1) );

	return mat;
}

Mat4x4 Anubis::CreatePerspectiveProjectionLH(	const AREAL fov,   const AREAL aspect,
												const AREAL nearZ, const AREAL farZ )
{
	Mat4x4 mat;

	//AREAL yscale = CtgR32(fov / 2);
	//AREAL xscale = yscale / aspect;

	AREAL SinFov = Sin(fov * 0.5f);
	AREAL CosFov = Cos(fov * 0.5f);

	AREAL height = CosFov / SinFov;
	AREAL width = height / aspect;

	mat.Init(	Vector(width, 0, 0, 0),
				Vector(0, height, 0, 0),
				Vector(0, 0, farZ/(farZ-nearZ), 1),
				Vector(0, 0, -nearZ*farZ/(farZ-nearZ), 0) );

	return mat;
}

Mat4x4 Anubis::CreateOrthoProjectionLH(	const AREAL width, const AREAL height,
										const AREAL nearZ,	const AREAL farZ)
{
	Mat4x4 mat;

	mat.Init(	Vector( 2.0f / width,			0,					0,					 0),
				Vector( 0,				 2.0f / height,				0,					 0),
				Vector( 0,						0,			1.0f / (farZ - nearZ),		 0),
				Vector( 0,						0,			nearZ / (nearZ - farZ),		 1) );

	return mat;
}

AREAL Anubis::Random(const AREAL min, const AREAL max)
{
	srand(0);

	AINT32 diff = max - min;
	return (rand() % diff) + min;
}

AREAL Anubis::Clamp(const AREAL value, const AREAL min, const AREAL max)
{
	if (value <= min) return min;
	if (value >= max) return max;
	return value;
}