#pragma once
//====================================================================================
//Camera.h
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

#include "../../../Math/Source/Geometry/Frustum.h"
#include "Final/Other/Viewport.h"

namespace Anubis
{
	class Camera
	{
	protected:
		/**
			View and Projection matrices
											**/
		Mat4x4	m_view;
		Mat4x4	m_prevView;
		Mat4x4	m_projection;
		Mat4x4	m_prevProjection;

		/**
			General Data
						 **/

		Vec	m_pos;		//position
		Vec m_dir;		//direction of the camera
		Vec m_initialDir;
		Vec	m_lookAt;	//point camera is looking at
		Vec m_up;		//direction of up vector

		AREAL m_yaw;
		AREAL m_pitch;
		AREAL m_roll;

		/**
			Projection Data
							**/
		Frustum	m_frustum;


		//Viewport to render to
		Viewport	m_viewport;

		/**
			Other Data
						**/
		ABOOL	m_bActive;
		ABOOL	m_bDebug;

		/** ===============================
					Methods
		=============================== **/
		AVOID UpdateTransforms();

	public:

		//Constructor and destructor
		Camera(	const Frustum & frustum,
				Vec & position, Vec & dir, Vec & up,
				AREAL yaw, AREAL pitch, AREAL roll,
				Viewport & viewport);
		~Camera();

		/*==
				Mutators
								==*/
		AINLINE AVOID SetActive(ABOOL isActive) { m_bActive = isActive; }

		AINLINE AVOID SetPosition	(const Vec & pos)	 { m_pos = pos; }
		AINLINE AVOID SetLookAt		(const Vec & lookAt) { m_lookAt = lookAt; }
		AINLINE AVOID SetDir		(const Vec & dir)	 { m_dir = dir; }

		AINLINE AVOID SetRoll		(const AREAL roll)	 { m_roll = roll; }
		AINLINE AVOID SetPitch		(const AREAL pitch)	 { m_pitch = pitch; }
		AINLINE AVOID SetYaw		(const AREAL yaw)	 { m_yaw = yaw; }

		/*==
				Accessors
								==*/

		AINLINE ABOOL IsActive() const { return m_bActive; }

		//AVIRTUAL HRESULT SetViewTransform(Scene *pScene);

		AINLINE const Frustum &GetFrustum() { return m_frustum; }

		AINLINE Mat4x4	GetProjection()		const { return m_projection; }
		AINLINE Mat4x4	GetView()			const { return m_view; }
		AINLINE Mat4x4&	GetViewProjection()	const { return m_view * m_projection; }
		AINLINE Mat4x4	GetPrevView()		const { return m_prevView; }
		AINLINE Mat4x4	GetPrevProjection()	const { return m_prevProjection; }

		AINLINE Mat4x4	GetOrthoProjection() const;

		AINLINE Vec		GetPosition()		const { return m_pos; }
		Vec				GetLookAt()			const;
		Vec				GetDir()			const { return m_dir; }

		AINLINE AREAL	GetRoll()			const { return m_roll; }
		AINLINE AREAL	GetPitch()			const { return m_pitch; }
		AINLINE AREAL	GetYaw()			const { return m_yaw; }

		AINLINE AREAL	GetNearZ()			const { return m_frustum.GetNearZ(); }
		AINLINE AREAL	GetFarZ()			const { return m_frustum.GetFarZ(); }

		AINLINE AINT32	GetViewportWidth()	const { return m_viewport.Width; }
		AINLINE AINT32	GetViewportHeight()	const { return m_viewport.Height; }

		/*==
				Methods
								==*/
		AVIRTUAL AVOID VUpdate(AUINT32 const elapsedMs);
	};

	//typedefs
	typedef shared_ptr<Camera>	CameraPtr;
	typedef vector<CameraPtr>	Cameras;
};