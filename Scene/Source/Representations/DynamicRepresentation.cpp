//====================================================================================
//DynamicRepresentation.cpp
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

#include "Scene_pch.h"
#include "DynamicRepresentation.h"

#include "../../../Math/Source/Interpolation.h"

using namespace Anubis;

AVOID DynamicRepresentation::SetCurrentTransform(Mat4x4 & mat, AREAL64 r64CurrentTime)
{
	m_prevWorldMatrix = m_currentWorldMatrix;
	m_currentWorldMatrix = mat;

	m_r64LastUpdate = r64CurrentTime;
}

AVOID DynamicRepresentation::SetBothTransforms(Mat4x4 & mat, AREAL64 r64CurrentTime)
{
	m_prevWorldMatrix = mat;
	m_currentWorldMatrix = mat;
	m_r64LastUpdate = r64CurrentTime;
}

//pre-rendering modifications
AVOID DynamicRepresentation::VPushParameters(Scene *pScene, AREAL64 r64CurrentTime)
{
	Mat4x4 transform;

	//new update cycle - current transform has changed
	if (m_prevWorldMatrix != m_currentWorldMatrix)
	{
		transform = LERP(m_prevWorldMatrix, m_currentWorldMatrix, min((r64CurrentTime - m_r64LastUpdate) / pScene->GetFixedTimeStep(), 1));
	}
	else
	{
		//we are not updating anything, so use current transform
		transform = m_currentWorldMatrix;
	}

	pScene->PushTransform(transform);
}