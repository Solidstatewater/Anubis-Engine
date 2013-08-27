//====================================================================================
//Camera.cpp
//
//This code is part of Anubis Engine.
//
//Anubis Engine is a free game engine created as a fan project to be
//awesome platform for developing games!
//
//All sources can be found here:
//	https://github.com/Dgek/Anubis-Engine
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
#include "Camera.h"

using namespace Anubis;

Camera::Camera(	const Frustum & frustum,
				Vec & position, Vec & dir, Vec & up,
				AREAL yaw, AREAL pitch, AREAL roll,
				Viewport & viewport/* = NULL */)
				:	
					m_frustum(frustum), m_bActive(true), m_bDebug(false),
					m_pos(position), m_up(up),
					m_yaw(yaw), m_pitch(pitch), m_roll(roll)
{
	m_dir = Normalize(dir);
	m_initialDir = m_dir;

	//initalize matrices
	UpdateTransforms();

	//assign viewport
	m_viewport = viewport;
}
Camera::~Camera()
{
	//SAFE_DELETE(m_pViewport);
}

AVOID Camera::UpdateTransforms()
{
	//m_roll = m_pitch = m_yaw = 0;
	//m_yaw = 0.05f;
	Mat4x4 rot;
	rot = rot.CreateRollPitchYaw(m_roll, m_pitch, m_yaw);

	//calculate point camera is focusing
	m_dir = m_initialDir * rot;
	m_dir = Normalize(m_dir);

	m_lookAt = m_pos + m_dir;

	//before calculating new view and proj matrices, update
	//previous values
	m_prevView = m_view;
	m_prevProjection = m_projection;

	//calculate view and projection matrices
	m_view			= CreateViewMatrixLH(m_pos, m_dir, m_up);
	m_projection	= CreatePerspectiveProjectionLH(	m_frustum.GetFOV(), m_frustum.GetAspectRatio(),
														m_frustum.GetNearZ(), m_frustum.GetFarZ() );
}

AVOID Camera::VUpdate(AUINT32 const elapsedMs)
{
	UpdateTransforms();
}

Mat4x4 Camera::GetOrthoProjection() const
{
	return CreateOrthoProjectionLH(SCREEN_WIDTH, SCREEN_HEIGHT, m_frustum.GetNearZ(), m_frustum.GetFarZ());
}

Vec Camera::GetLookAt() const
{
	Mat4x4 rot;
	rot = rot.CreateRollPitchYaw(m_roll, m_pitch, m_yaw);

	Vec lookAt;
	lookAt = m_pos + m_dir * rot;

	return lookAt;
}