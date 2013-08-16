#pragma once
//====================================================================================
//Frustum.h : frustum geometry
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

#include "Plane.h"

namespace Anubis
{
	class Frustum
	{
	public:
		enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

		Plane	m_planes[NumPlanes];	// planes of the frusum in camera space
		Vec		m_nearClip[4];			// verts of the near clip plane in camera space
		Vec		m_farClip[4];			// verts of the far clip plane in camera space

		AREAL m_fov;				// field of view in radians
		AREAL m_aspect;				// aspect ratio - width divided by height
		AREAL m_near;				// near clipping distance
		AREAL m_far;				// far clipping distance

		AREAL m_fNearPlaneWidth;
		AREAL m_fNearPlaneHeight;

	public:

		//constructor
		Frustum();
		~Frustum();

		/**
				Initializers
								**/
		AVOID Init(const AREAL fov, const AREAL aspect, const AREAL near, const AREAL far);

		/**
				Methods
							**/
		ABOOL Inside(const Vec & point) const;
		ABOOL Inside(const Vec & point, const AREAL radius) const;

		/**
				Getters
							**/
		const Plane &	Get(Side side)		const { return m_planes[side]; }
		const AREAL		GetFOV()			const { return m_fov; }
		const AREAL		GetAspectRatio()	const { return m_aspect; }
		const AREAL		GetNearZ()			const { return m_near; }
		const AREAL		GetFarZ()			const { return m_far; }
		const AREAL		GetNearWidth()		const { return m_fNearPlaneWidth; }
		const AREAL		GetNearHeight()		const { return m_fNearPlaneHeight; }

		/**
				Mutators
							**/
		AVOID SetFOV(AREAL fov)			{ m_fov=fov;		Init(m_fov, m_aspect, m_near, m_far); }
		AVOID SetAspect(AREAL aspect)	{ m_aspect=aspect;	Init(m_fov, m_aspect, m_near, m_far); }
		AVOID SetNear(AREAL nearClip)	{ m_near=nearClip;	Init(m_fov, m_aspect, m_near, m_far); }
		AVOID SetFar(AREAL farClip)		{ m_far=farClip;	Init(m_fov, m_aspect, m_near, m_far); }

		void Render();
	};
};