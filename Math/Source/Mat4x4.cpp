//====================================================================================
//Mat4x4.cpp : matrix class implemenatation
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
#include "Mat4x4.h"

using namespace Anubis;

///////////////////////////////////////////
//Mat4x4 definition
///////////////////////////////////////////
//Vec Mat4x4::Transform(Vec v)
//{
	/*Vec row1 = _mm_mul_ps(_mm_replicate_x_ps(v), rows[0]);
	Vec row2 = _mm_mul_ps(_mm_replicate_y_ps(v), rows[1]);
	Vec row3 = _mm_mul_ps(_mm_replicate_z_ps(v), rows[2]);
	Vec row4 = _mm_mul_ps(_mm_replicate_w_ps(v), rows[3]);

	Vec res = _mm_add_ps(row1, row2);
	res = _mm_add_ps(res, row3);
	res = _mm_add_ps(res, row4); */

//	Vec res = _mm_mul_ps(_mm_replicate_x_ps(v), rows[0]);
//	res = _mm_madd_ps(_mm_replicate_y_ps(v), rows[1], res);
//	res = _mm_madd_ps(_mm_replicate_z_ps(v), rows[2], res);
//	res = _mm_madd_ps(_mm_replicate_w_ps(v), rows[3], res);

//	return res;
//}

///////////////////////////////////////////
//Constructors and Initializers
///////////////////////////////////////////
/*Mat4x4::Mat4x4()
{
	rows[0] = _mm_set_ps(1, 0, 0, 0);
	rows[1] = _mm_set_ps(0, 1, 0, 0);
	rows[2] = _mm_set_ps(0, 0, 1, 0);
	rows[3] = _mm_set_ps(0, 0, 0, 1);
} */

/*ABOOL Mat4x4::Init(	Vec & v0,
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
}
*/
///////////////////////////////////////////
//Static methods
///////////////////////////////////////////
/*Mat4x4 Mat4x4::Identity()
{
	Mat4x4 mat;
	mat.Init(	Vector(1, 0, 0, 0),
				Vector(0, 1, 0, 0),
				Vector(0, 0, 1, 0),
				Vector(0, 0, 0, 1)	); //create identity matrix

	return mat;
} */

Mat4x4 Mat4x4::CreateTranslation(const Vec & vec)
{
	rows[0] = Vector(1, 0, 0, 0);
	rows[1] = Vector(0, 1, 0, 0);
	rows[2] = Vector(0, 0, 1, 0);
	rows[3] = vec;
	
	return *this;
}

Mat4x4 Mat4x4::CreateRotationX(AREAL angleInRadians)
{
	rows[0] = Vector(1, 0, 0, 0);
	rows[1] = Vector(0, Cos(angleInRadians), Sin(angleInRadians), 0);
	rows[2] = Vector(0, -Sin(angleInRadians), Cos(angleInRadians), 0);
	rows[3] = Vector(0, 0, 0, 1);
	
	return *this;
}

Mat4x4 Mat4x4::CreateRotationY(AREAL angleInRadians)
{
	rows[0] = Vector(Cos(angleInRadians), 0, -Sin(angleInRadians), 0);
	rows[1] = Vector(0, 1, 0, 0);
	rows[2] = Vector(Sin(angleInRadians), 0, Cos(angleInRadians), 0);
	rows[3] = Vector(0, 0, 0, 1);
	
	return *this;
}

Mat4x4 Mat4x4::CreateRotationZ(AREAL angleInRadians)
{
	rows[0] = Vector(Cos(angleInRadians), Sin(angleInRadians), 0, 0);
	rows[1] = Vector(-Sin(angleInRadians), Cos(angleInRadians), 0, 0);
	rows[2] = Vector(0, 0, 1, 0);
	rows[3] = Vector(0, 0, 0, 1);
	
	return *this;
}

Mat4x4 Mat4x4::CreateScaling(Vec & scale)
{
	rows[0] = Vector(getx(scale), 0, 0, 0);
	rows[1] = Vector(0, gety(scale), 0, 0);
	rows[2] = Vector(0, 0, getz(scale), 0);
	rows[3] = Vector(0, 0, 0, 1);

	return *this;
}

Mat4x4 Mat4x4::CreateRollPitchYaw(AREAL rollInRadians, AREAL pitchInRadians, AREAL yawInRadians)
{
	return CreateRotationZ(rollInRadians) * CreateRotationX(pitchInRadians) * CreateRotationY(yawInRadians);
}

///////////////////////////////////////////
//Operations on matrices
///////////////////////////////////////////
AVOID Mat4x4::Transpose()
{
	//Calculate new rows values
	Vec r0 = Vector(getx(rows[0]), getx(rows[1]), getx(rows[2]), getx(rows[3]));
	Vec r1 = Vector(gety(rows[0]), gety(rows[1]), gety(rows[2]), gety(rows[3]));
	Vec r2 = Vector(getz(rows[0]), getz(rows[1]), getz(rows[2]), getz(rows[3]));
	Vec r3 = Vector(getw(rows[0]), getw(rows[1]), getw(rows[2]), getw(rows[3]));

	//Assign values to the matrix array
	rows[0] = r0;
	rows[1] = r1;
	rows[2] = r2;
	rows[3] = r3;
}
AVOID Mat4x4::Inverse()
{
    int indxc[4], indxr[4];
    int ipiv[4] = { 0, 0, 0, 0 };
    float minv[4][4];
    //memcpy(minv, m.m, 4*4*sizeof(float));
	for (int i = 0; i < 4; i++)
	{
		minv[i][0] = rows[i].x;
		minv[i][1] = rows[i].y;
		minv[i][2] = rows[i].z;
		minv[i][3] = rows[i].w;
	}

    for (int i = 0; i < 4; i++) {
        int irow = -1, icol = -1;
        float big = 0.;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (fabsf(minv[j][k]) >= big) {
                            big = float(fabsf(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (ipiv[k] > 1)
                        assert(0 && "Attempt to calculate inverseof singular matrix!!!");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k)
                swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.)
            assert(0 && "Attempt to calculate inverseof singular matrix!!!");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1.f / minv[icol][icol];
        minv[icol][icol] = 1.f;
        for (int j = 0; j < 4; j++)
            minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++)
                    minv[j][k] -= minv[icol][k]*save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }

	//assign values
	rows[0] = Vector(minv[0][0], minv[0][1], minv[0][2], minv[0][3]);
	rows[1] = Vector(minv[1][0], minv[1][1], minv[1][2], minv[1][3]);
	rows[2] = Vector(minv[2][0], minv[2][1], minv[2][2], minv[2][3]);
	rows[3] = Vector(minv[3][0], minv[3][1], minv[3][2], minv[3][3]);
}

AREAL Mat4x4::GetElement(const AUINT8 row, const AUINT8 col) const
{
	if (col == 0) return getx(rows[row]);
	if (col == 1) return gety(rows[row]);
	if (col == 2) return getz(rows[row]);
	if (col == 3) return getw(rows[row]);
}

///////////////////////////////////////////
//Operators
///////////////////////////////////////////

Mat4x4 Mat4x4::operator+(const Mat4x4 & M) const
{
	Mat4x4 mat;
	mat.rows[0] = rows[0] + M.rows[0];
	mat.rows[1] = rows[1] + M.rows[1];
	mat.rows[2] = rows[2] + M.rows[2];
	mat.rows[3] = rows[3] + M.rows[3];

	return mat;
}

Mat4x4 Mat4x4::operator*(const Mat4x4 & M) const
{
	Mat4x4 mat;
	mat.rows[0] = rows[0] * M;
	mat.rows[1] = rows[1] * M;
	mat.rows[2] = rows[2] * M;
	mat.rows[3] = rows[3] * M;

	return mat;
}

/*************	Matrix Stack Implementation	 *************/
MatrixStack::MatrixStack()
{
	m_matrices.push_back(Mat4x4::Identity());
	m_count = 1;
}

MatrixStack::~MatrixStack()
{
	//clear the stack
	while (Pop());
}

ABOOL MatrixStack::Pop()
{
	if (m_count)
	{
		m_matrices.pop_back();
		m_count--;
		
		return true;
	}

	//no matrices in the stack!
	return false;
}

AVOID MatrixStack::PushTransform(const Mat4x4 & mat)
{
	m_matrices.push_back(m_matrices.back() * mat);
	m_count++;
}