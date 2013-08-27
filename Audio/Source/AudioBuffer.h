#pragma once
//====================================================================================
//AudioBuffer.h
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


#include "Resource.h"

namespace Anubis
{
	class IAudioBuffer
	{
	public:
		
		AVIRTUAL ~IAudioBuffer() {}

		AVIRTUAL APVOID VGet() = 0;
		AVIRTUAL shared_ptr<ResHandle> VGetResource() = 0;
		AVIRTUAL ABOOL VRestore() = 0;
		
		AVIRTUAL ABOOL VPlay(AINT32 volume, ABOOL looping) = 0;
		AVIRTUAL ABOOL VPause() = 0;
		AVIRTUAL ABOOL VStop() = 0;
		AVIRTUAL ABOOL VResume() = 0;

		AVIRTUAL ABOOL VTogglePause() = 0;
		AVIRTUAL ABOOL VIsPlaying() = 0;
		AVIRTUAL ABOOL VIsLooping() = 0;
		AVIRTUAL AVOID VSetVolume(AINT32 volume) = 0;

		AVIRTUAL AINT32 VGetVolume() const = 0;
		AVIRTUAL AREAL  VGetProgress() = 0;
	};

	class AudioBuffer : public IAudioBuffer
	{
	protected:
		std::shared_ptr<ResHandle> m_pResource;

		ABOOL m_bPaused;
		ABOOL m_bLooping;
		AINT32 m_iVolume;

	public:
		AudioBuffer(std::shared_ptr<ResHandle> pResource)
		{
			m_pResource = pResource;

			m_bPaused = false;
			m_bLooping = false;
			m_iVolume = 0;
		}

		AVIRTUAL std::shared_ptr<ResHandle> VGetResource() { return m_pResource; }
		AVIRTUAL ABOOL VIsLooping() { return m_bLooping; }
		AVIRTUAL AINT32 VGetVolume() const { return m_iVolume; }
	};
};