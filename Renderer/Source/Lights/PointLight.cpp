//====================================================================================
//PointLight.cpp
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
#include "Renderer_pch.h"
#include "PointLight.h"
#include "../Renderer.h"
#include <Final\General.h>
#include "../../Math/Source/Math.h"

using namespace Anubis;

AVOID PointLight::VPreRender(Renderer *pRenderer)
{
	Light::VPreRender(pRenderer);

	//bind point light specific data
	Vec range = Vector(m_r32Range, m_r32Range, m_r32Range, m_r32Range);
	pRenderer->m_pcbPointLight->UpdateSubresource(0, NULL, &range, 0, 0);
//	pRenderer->m_pcbPointLight->UpdateSubresource(0, nullptr, &m_vec, 0, 0);
	pRenderer->m_pcbPointLight->Set(2, ST_Pixel);
}

AVOID PointLight::VPostRender(Renderer* pRenderer)
{
	Light::VPostRender(pRenderer);
	D3D11DeviceContext()->RSSetScissorRects(0, nullptr);
}

AVOID PointLight::VSetScissorRect(Renderer* pRenderer, CameraPtr pCamera)
{
	Vec vCenterWS = m_pData->m_pos;

	//convert center to view space
	Vec vCenterVS = vCenterWS * pCamera->GetView();

	//calculate four points (top, bottom, left, right) around the light
	Vec vTopVS = vCenterVS + Vector(0.0f, m_r32Range, 0.0f, 0.0f);
	Vec vBottomVS = vCenterVS - Vector(0.0f, m_r32Range, 0.0f, 0.0f);
	Vec vLeftVS = vCenterVS - Vector(m_r32Range, 0.0f, 0.0f, 0.0f);
	Vec vRightVS = vCenterVS + Vector(m_r32Range, 0.0f, 0.0f, 0.0f);

	//decide which points we take to bound whole light volume to the quad
	vTopVS.z	= vTopVS.y < 0.0f ? vTopVS.z + m_r32Range : vTopVS.z - m_r32Range;
	vBottomVS.z = vBottomVS.y < 0.0f ? vBottomVS.z - m_r32Range : vBottomVS.z + m_r32Range;
	vLeftVS.z	= vLeftVS.x < 0.0f ? vLeftVS.z - m_r32Range : vLeftVS.z + m_r32Range;
	vRightVS.z	= vRightVS.x < 0.0f ? vRightVS.z + m_r32Range : vRightVS.z - m_r32Range;

	//clamp positions to near and far frustum planes
	AREAL r32Near = pCamera->GetNearZ();
	AREAL r32Far = pCamera->GetFarZ();

	vTopVS.z	= Clamp(vTopVS.z, r32Near, r32Far);
	vBottomVS.z = Clamp(vBottomVS.z, r32Near, r32Far);
	vLeftVS.z	= Clamp(vLeftVS.z, r32Near, r32Far);
	vRightVS.z	= Clamp(vRightVS.z, r32Near, r32Far);

	//now transform to clip space
	Mat4x4 & proj = pCamera->GetProjection();

	//!! perspective projection matrix has following view,
	//so, we can easily economize computations
	// s		0		0			0
	// 0		s		0			0
	// 0		0		-f/(f-n)	-1
	// 0		0		-f*n/(f-n)	0
	AREAL topCS		= vTopVS.y * proj.GetElement(1, 1) / vTopVS.z;
	AREAL bottomCS	= vBottomVS.y * proj.GetElement(1, 1) / vBottomVS.z;
	AREAL leftCS	= vLeftVS.x * proj.GetElement(0, 0) / vLeftVS.z;
	AREAL rightCS	= vRightVS.x * proj.GetElement(0, 0) / vRightVS.z;

	//clamp coordinates
	topCS		= Clamp(topCS, -1.0f, 1.0f);
	bottomCS	= Clamp(bottomCS, -1.0f, 1.0f);
	leftCS		= Clamp(leftCS, -1.0f, 1.0f);
	rightCS		= Clamp(rightCS, -1.0f, 1.0f);

	//convert to screen coordinates
	AREAL topSS		= (topCS -1.0f) * (-0.5f);
	AREAL bottomSS	= (bottomCS - 1.0f) * (-0.5f);
	AREAL leftSS	= (leftCS + 1.0f) * 0.5f;
	AREAL rightSS	= (rightCS + 1.0f) * 0.5f;

	//viewport coordinates
	topSS		*= pCamera->GetViewportHeight();
	bottomSS	*= pCamera->GetViewportHeight();
	leftSS		*= pCamera->GetViewportWidth();
	rightSS		*= pCamera->GetViewportWidth();

	//A little bit API specific code here...
	D3D11_RECT scissorRect;
	scissorRect.top		= static_cast<AINT32>(topSS);
	scissorRect.bottom	= static_cast<AINT32>(bottomSS);
	scissorRect.left	= static_cast<AINT32>(leftSS);
	scissorRect.right	= static_cast<AINT32>(rightSS);

	//clamp values, yes again...
	scissorRect.top		= max(0.0f, scissorRect.top);
	scissorRect.bottom	= min(static_cast<AINT32>(pCamera->GetViewportHeight()), scissorRect.bottom);
	scissorRect.left	= max(0.0f, scissorRect.left);
	scissorRect.right	= min(static_cast<AINT32>(pCamera->GetViewportWidth()), scissorRect.right);

	D3D11DeviceContext()->RSSetScissorRects(1, &scissorRect);
}
