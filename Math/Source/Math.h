#pragma once
//====================================================================================
//Math.h
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

#include <math.h>
#include "Mat4x4.h"

namespace Anubis
{
	/*************************************
				Trigonometry
	*************************************/
	AREAL32 CosR32( AREAL32 angleInRadians );
	AREAL64 CosR64( AREAL64 angleInRadians );
	AREAL64 Cos	  ( AREAL64 angleInRadians );

	AREAL32 SinR32( AREAL32 angleInRadians );
	AREAL64 SinR64( AREAL64 angleInRadians );
	AREAL64 Sin	  ( AREAL64 angleInRadians );

	AREAL32 TgR32 ( AREAL32 angleInRadians );
	AREAL64 TgR64 ( AREAL64 angleInRadians );
	AREAL64 Tg	  ( AREAL64 angleInRadians );

	AREAL32 CtgR32( AREAL32 angleInRadians );
	AREAL64 CtgR64( AREAL64 angleInRadians );
	AREAL64 Ctg	  ( AREAL64 angleInRadians );

	AREAL32 Atan2R32( AREAL32 y, AREAL32 x );
	AREAL64 Atan2R64( AREAL64 y, AREAL32 x );
	AREAL64 Atan2	( AREAL64 y, AREAL64 x );

	//AREAL64 Dot(Vec v1, Vec v2) { 
	AREAL32 SqrtR32(AREAL32 x);
	AREAL64 SqrtR64(AREAL64 x);
	AREAL64 Sqrt(AREAL64 x);

	/* ======================================
					Vectors
	========================================= */
	AREAL Dot(const Vec& v1, const Vec& v2);
	AREAL Length(const Vec & v);
	Vec Normalize(const Vec & v);

	/* ======================================
					Matrices
	========================================= */
	Mat4x4 CreateViewMatrixLH(const Vec & pos, const Vec & lookAt, const Vec & up);
	Mat4x4 CreatePerspectiveProjectionLH(	const AREAL fov,	const AREAL aspect,
											const AREAL nearZ,	const AREAL farZ );
	Mat4x4 CreateOrthoProjectionLH(	const AREAL width,	const AREAL height,
									const AREAL nearZ,	const AREAL farZ);

	/* ======================================
				Random Number Generator
	========================================= */
	AREAL Random(const AREAL min, const AREAL max);

	/* ======================================
						Other
	========================================= */
	AREAL Clamp(const AREAL value, const AREAL min, const AREAL max);
};