#include <cstdlib>
#include <iostream>

#include <Anubis.h>

#include "Allocator.h"
#include "../Math/Source/Vectors.h"
#include "../Math/Source/Mat4x4.h"
//#include "../Renderer/Source/Cameras/Camera.h"

#include <d3d11.h>
#include <D3DX11.h>

#include <Windows.h>
#include <memory>
#include <vector>

#define ADX11_API

using namespace std;
using namespace Anubis;

void output(Vec v)
{
	cout << "x: " << getx(v) << " y: " << gety(v) << " z: " <<
		getz(v) << " w: " << getw(v) << endl;

	cout << "r: " << getr(v) << " g: " << getg(v) << " b: " <<
		getb(v) << " a: " << geta(v) << endl;
};

int main()
{
	cout << "Vec alignment: " << __alignof(Vec) << endl;
	cout << "Mat4x4 alignment: " << __alignof(Mat4x4) << endl;
	cout << "MatrixStack alignment: " << __alignof(MatrixStack) << endl;
	cout << "float2 alignment: " << __alignof(float2) << endl;
	cout << "float3 alignment: " << __alignof(float3) << endl;
	cout << "float4 alignment: " << __alignof(float4) << endl;
	//cout << "Camera alignment: " << __alignof(Camera) << endl;


	Vec v1, v2;

	v1 = Vector(1.0f, 2.0f, 3.0f, 4.0f);
	ALIGN16 AREAL f[4] = { 2.0f, 3.0f, 4.0f, 5.0f };
	v2 = Vector(f);
	//v2 = Vector(2.0f, 3.0f, 4.0f, 5.0f);

	Vec v3 = v1 + v2;
	Vec v4 = v1 * v2;

	output(v3);
	output(v4);

	//Some matrix multiplication..
	Mat4x4 m1 = Mat4x4::Identity();
	Vec v5 = v4 * m1;

	output(v5);

	system("pause");
}