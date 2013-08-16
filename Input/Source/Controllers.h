#pragma once
//====================================================================================
//Controllers.h
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

#include "Devices.h"
#include "../../Math/Source/Math_pch.h"
#include "../../Math/Source/Standard.h"

namespace Anubis
{
	class MovementController
	{
	public:
		AVIRTUAL AREAL	VGetTargetYaw()			const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetPitch()		const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetRoll()		const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetYawRot()		const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetPitchRot()	const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetRollRot()		const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetAngularSpeed()const	{ return 0.0f; }
		AVIRTUAL AREAL	VGetTargetSpeed()		const	{ return 0.0f; }
		AVIRTUAL Vec	VGetTargetDirection()	const	{ return Vector(0.0f, 0.0f, 1.0f, 0.0f); }
		AVIRTUAL Vec	VGetTargetVelocity()	const	{ return Vector(0.0f, 0.0f, 0.0f, 0.0f); }

		AVIRTUAL AVOID	VSetTargetDirection(const Vec & vDir) = 0;

		AVIRTUAL AVOID VUpdate(AINT32 const deltaMs) {}
	};

	class PCPlayerController : public IMouseHandler, public IKeyboardHandler
	{
	protected:

		//key codes
		Key				m_bKey[256];

		//previous mouse position
		float2			m_prevMousePos;
		Point			m_screenCenter;

	public:

		PCPlayerController()
			{
				for (auto i = 0; i < 256; i++)
				{
					m_bKey[i] = false;
				}

				//m_screenCenter = Point(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
				m_screenCenter = Point(960, 540);
				SetCursorPos(m_screenCenter.x(), m_screenCenter.y());
			}

		ABOOL VOnLButtonDown(const AREAL x, const AREAL y)	{ return true; }
		ABOOL VOnLButtonUp(const AREAL x, const AREAL y)	{ return true; }

		ABOOL VOnRButtonDown(const AREAL x, const AREAL y)	{ return true; }
		ABOOL VOnRButtonUp(const AREAL x, const AREAL y)	{ return true; }

		ABOOL VOnMouseMove(const AREAL x, const AREAL y)	{ return true; }

		ABOOL VOnKeyDown(Key const key)
			{
				m_bKey[key] = true;

				return true;
			}
		ABOOL VOnKeyUp(Key const key)
			{
				m_bKey[key] = false;

				return true;
			}
	};

	class FreeCameraController : public PCPlayerController, public MovementController
	{
	protected:
		AREAL	m_r32TargetYawRot;
		AREAL	m_r32TargetPitchRot;
		AREAL	m_r32TargetRollRot;
		AREAL	m_r32TargetYaw;
		AREAL	m_r32TargetPitch;
		AREAL	m_r32TargetRoll;

		Vec		m_vTargetDirection;
		
		AREAL	m_r32MaxSpeed;
		AREAL	m_r32AngularSpeed;
		AREAL	m_r32CurrentSpeed;
		AREAL	m_r32TargetSpeed;
		Vec		m_vTargetVelocity;

	public:

		//Contructor
		FreeCameraController()
			{
				m_r32TargetYaw = 0.0f;
				m_r32TargetPitch = 0.0f;
				m_r32TargetRoll = 0.0f;

				m_r32TargetYawRot = 0.0f;
				m_r32TargetPitchRot = 0.0f;
				m_r32TargetRollRot = 0.0f;

				m_vTargetDirection = Vector(0.0f, 0.0f, 1.0f, 0.0f);

				m_r32TargetSpeed = 1.0f;
				m_vTargetVelocity = Vector(0.0f, 0.0f, 0.0f, 0.0f);
			}

		// Mouse Handler Implementation //
		ABOOL VOnMouseMove(const AREAL x, const AREAL y)
			{
				/*if (x != m_screenCenter.x() || y != m_screenCenter.y())
				{
					m_r32TargetYawRot = (x - m_screenCenter.x());
					m_r32TargetPitchRot = -(y - m_screenCenter.y());

					m_r32TargetYaw += m_r32TargetYawRot;
					m_r32TargetPitch += m_r32TargetPitchRot;

					SetCursorPos(m_screenCenter.x(), m_screenCenter.y());

					m_prevMousePos.x = x;
					m_prevMousePos.y = y;
				} */

				if (x != m_prevMousePos.x || y != m_prevMousePos.y)
				{
					m_r32TargetYawRot = (x - m_prevMousePos.x) * 0.01f;
					m_r32TargetPitchRot = -(y - m_prevMousePos.y) * 0.01f;

					m_r32TargetYaw += m_r32TargetYawRot;
					m_r32TargetPitch += m_r32TargetPitchRot;

					//SetCursorPos(m_screenCenter.x(), m_screenCenter.y());

					m_prevMousePos.x = x;
					m_prevMousePos.y = y;
				} 

				return true;
			}

		//Keyboard handler Implementation //

		// Movement controller implementation //
		AREAL	VGetTargetYaw()			const	{ return m_r32TargetYaw; }
		AREAL	VGetTargetPitch()		const	{ return m_r32TargetPitch; }
		AREAL	VGetTargetRoll()		const	{ return m_r32TargetRoll; }
		AREAL	VGetTargetYawRot()		const	{ return m_r32TargetYawRot; }
		AREAL	VGetTargetPitchRot()	const	{ return m_r32TargetPitchRot; }
		AREAL	VGetTargetRollRot()		const	{ return m_r32TargetRollRot; }
		AREAL	VGetTargetSpeed()		const	{ return m_r32TargetSpeed; }
		Vec 	VGetTargetVelocity()	const	{ return m_vTargetVelocity; }

		AVOID	VSetTargetDirection(const Vec & vDir) { m_vTargetDirection = vDir; }

		ABOOL VOnKeyDown(Key const key)
			{
				m_bKey[key] = true;

				switch (key)
				{
					case 'W':
						m_vTargetVelocity = m_vTargetDirection * m_r32TargetSpeed;
						break;
					case 'S':
						m_vTargetVelocity = -m_vTargetDirection * m_r32TargetSpeed;
						break;
					case 'A':
						{
							Mat4x4 rot;
							rot.CreateRotationY(-Pi / 2.0);
							Vec left = m_vTargetDirection * rot;
							m_vTargetVelocity = left * m_r32TargetSpeed;
							break;
						}
					case 'D':
						{
							Mat4x4 rot;
							rot.CreateRotationY(Pi / 2.0);
							Vec right = m_vTargetDirection * rot;
							m_vTargetVelocity = right * m_r32TargetSpeed;
							break;
						}
				}

				return true;
			}

		AVOID VUpdate(AINT32 const deltaMs)
			{
				if (!m_bKey['W'] && !m_bKey['S'] && !m_bKey['A'] && !m_bKey['D'])
				{
					m_vTargetVelocity = Vector(0.0f, 0.0f, 0.0f, 0.0f);
				}
			}
	};
}; //Anubis