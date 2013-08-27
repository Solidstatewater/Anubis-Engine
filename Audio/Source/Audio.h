#pragma once
//====================================================================================
//Audio.h
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

#include "AudioBuffer.h"
#include "Resource.h"

namespace Anubis
{
	enum SoundType
	{
		ST_wave,
		ST_Count,
	};

	class SoundResourceExtraData : public IResourceExtraData
	{
		friend class WaveResourceLoader;

	public: 	
		SoundResourceExtraData();
		AVIRTUAL ~SoundResourceExtraData() { }
		AVIRTUAL std::string VToString() { return "SoundResourceExtraData"; }
		enum SoundType GetSoundType() { return m_SoundType; }
		WAVEFORMATEX const *GetFormat() { return &m_WavFormatEx; }
		AINT32 GetLengthMilli() const { return m_LengthMilli; }

	protected:
		enum SoundType m_SoundType;			
		ABOOL m_bInitialized;			
		WAVEFORMATEX m_WavFormatEx;		
		AINT32 m_LengthMilli;					
	};
	
	class IAudio
	{
	public:
		AVIRTUAL ABOOL VActive() = 0;

		AVIRTUAL IAudioBuffer* VInitAudioBuffer(std::shared_ptr<ResHandle> handle) = 0;
		AVIRTUAL AVOID VReleaseAudioBuffer(IAudioBuffer* audioBuffer) = 0;

		AVIRTUAL AVOID VStopAllSounds() = 0;
		AVIRTUAL AVOID VPauseAllSounds() = 0;
		AVIRTUAL AVOID VResumeAllSounds() = 0;

		AVIRTUAL ABOOL VInitialize(HWND hWnd) = 0;
		AVIRTUAL AVOID VShutDown() = 0;
	};

	class Audio : public IAudio
	{
	protected:

		typedef std::list<IAudioBuffer*> AudioBuffers;
		AudioBuffers m_AllSamples;
		ABOOL m_bAllPaused;
		ABOOL m_bInitialized;

	public:
		Audio();

		AVIRTUAL AVOID VStopAllSounds();
		AVIRTUAL AVOID VPauseAllSounds();
		AVIRTUAL AVOID VResumeAllSounds();

		AVIRTUAL AVOID VShutDown();

		ASTATIC ABOOL HasSoundCard();
		ABOOL IsPaused() { m_bAllPaused; }
	};

	extern IAudio* g_pAudio;
};