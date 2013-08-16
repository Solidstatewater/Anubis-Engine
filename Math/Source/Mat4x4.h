#pragma once
//====================================================================================
//Mat4x4.h : declare matrix class
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

#include "Macroses.h"
#include "Vectors.h"

using namespace std;

namespace Anubis
{
	/***
		Define Matrix 4x4 structure
	***/
	typedef struct Mat4x4 
	{
	public:
			Vec rows[4];

	public:
		/**								==
				Static methods
		==								**/
		ASTATIC Mat4x4 Identity()
		{
			Mat4x4 mat;
			mat.Init(	Vector(1, 0, 0, 0),
						Vector(0, 1, 0, 0),
						Vector(0, 0, 1, 0),
						Vector(0, 0, 0, 1)	); //create identity matrix
	
			return mat;
		}

		/**
			Costructors and Initializers
											**/
		Mat4x4()
		{
			#ifdef SIMD_MATH_ENABLED
				rows[0] = _mm_set_ps(0, 0, 0, 1);
				rows[1] = _mm_set_ps(0, 0, 1, 0);
				rows[2] = _mm_set_ps(0, 1, 0, 0);
				rows[3] = _mm_set_ps(1, 0, 0, 0);
			#else
				rows[0] = Vector(1, 0, 0, 0);
				rows[1] = Vector(0, 1, 0, 0);
				rows[2] = Vector(0, 0, 1, 0);
				rows[3] = Vector(0, 0, 0, 1);
			#endif
		}

		Mat4x4( const AREAL _x00, const AREAL _x01, const AREAL _x02, const AREAL _x03,
				const AREAL _x10, const AREAL _x11, const AREAL _x12, const AREAL _x13,
				const AREAL _x20, const AREAL _x21, const AREAL _x22, const AREAL _x23,
				const AREAL _x30, const AREAL _x31, const AREAL _x32, const AREAL _x33 )
		{
#ifdef SIMD_MATH_ENABLED
			rows[0] = _mm_set_ps(_x00, _x01, _x02, _x03);
			rows[1] = _mm_set_ps(_x10, _x11, _x12, _x13);
			rows[2] = _mm_set_ps(_x20, _x21, _x22, _x23);
			rows[3] = _mm_set_ps(_x30, _x31, _x32, _x33);
#else
			rows[0] = Vector(_x00, _x01, _x02, _x03);
			rows[1] = Vector(_x10, _x11, _x12, _x13);
			rows[2] = Vector(_x20, _x21, _x22, _x23);
			rows[3] = Vector(_x30, _x31, _x32, _x33);
#endif
		}

		ABOOL Init(	Vec & v0,
					Vec & v1,
					Vec & v2,
					Vec & v3)
					{
						//Copy vector values
						rows[0] = v0;
						rows[1] = v1;
						rows[2] = v2;
						rows[3] = v3;

						return true;
					};

		//Create translation matrix
		Mat4x4 CreateTranslation(const Vec & vec);

		//Create rotation matrix
		Mat4x4 CreateRotationX(AREAL angleInRadians);
		Mat4x4 CreateRotationY(AREAL angleInRadians);
		Mat4x4 CreateRotationZ(AREAL angleInRadians);

		Mat4x4 CreateRollPitchYaw(AREAL rollInRadians, AREAL pitchInRadians, AREAL yawInRadians);

		//create scaling matrix
		Mat4x4 CreateScaling(Vec & scale);
		Mat4x4 CreateScaling(AREAL x, AREAL y, AREAL z); //more effective

		AREAL  GetElement(const AUINT8 row, const AUINT8 col) const;

		/**
				Operations on matrices
											**/
		AVOID Inverse();
		AVOID Transpose();

		Mat4x4 operator+(const Mat4x4 & M) const;
		Mat4x4 operator*(const Mat4x4 & M) const;

	} Mat4x4, *pMat4x4;

	//define matrix list
	typedef vector<Mat4x4> Matrices;

	//----------------------------------------------------
	//Operators
	//----------------------------------------------------
	AINLINE Vec operator*(Vec v, const Mat4x4 & M)
	{
		#ifdef SIMD_MATH_ENABLED
			Vec res;

			//calc first row
			res = _mm_mul_ps ( _mm_replicate_x_ps(v), M.rows[0]);
			//calc second row and add result
			res = _mm_madd_ps( _mm_replicate_y_ps(v), M.rows[1], res);
			//calc third row and add result
			res = _mm_madd_ps( _mm_replicate_z_ps(v), M.rows[2], res);
			//calc fourth row and add result
			res = _mm_madd_ps( _mm_replicate_w_ps(v), M.rows[3], res);

			return res;
		#else
			return Vec(	v.x * M.rows[0].x + v.y * M.rows[1].x + v.z * M.rows[2].x + v.w * M.rows[3].x,
						v.x * M.rows[0].y + v.y * M.rows[1].y + v.z * M.rows[2].y + v.w * M.rows[3].y,
						v.x * M.rows[0].z + v.y * M.rows[1].z + v.z * M.rows[2].z + v.w * M.rows[3].z,
						v.x * M.rows[0].w + v.y * M.rows[1].w + v.z * M.rows[2].w + v.w * M.rows[3].w);
		#endif
} 

	AINLINE ABOOL operator==(const Mat4x4 & m1, const Mat4x4 & m2)
	{
		return (m1.rows[0] == m2.rows[0] &&
				m1.rows[1] == m2.rows[1] &&
				m1.rows[2] == m2.rows[2] &&
				m1.rows[3] == m2.rows[3] );
	}
	AINLINE ABOOL operator!=(const Mat4x4 & m1, const Mat4x4 & m2)
	{
		return (!(	m1.rows[0] == m2.rows[0] &&
					m1.rows[1] == m2.rows[1] &&
					m1.rows[2] == m2.rows[2] &&
					m1.rows[3] == m2.rows[3] ));
	}


	//Use this class for connectiing affine transformations
	class MatrixStack
	{
	private:
		//matrix stack itself
		Matrices	m_matrices;
		AUINT8		m_count; //probably 256 matrices in the stack is more than enough

	public:
		//Constructor and Destructor
		MatrixStack();
		~MatrixStack();

		//Get top(current) matrix
		Mat4x4 Get() const { return m_matrices.back(); }

		//Get number of matrices in the stack
		AUINT8 Count() const { return m_count; }

		//remove current matrix
		ABOOL Pop();

		//Add transform to the current matrix and add new one
		AVOID PushTransform(const Mat4x4 & mat);
	};

}; //Anubis