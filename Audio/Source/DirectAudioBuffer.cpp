//====================================================================================
//DirectAudioBuffer.cpp
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

#include "Audio_pch.h"
#include "Audio.h"
#include "DirectAudioBuffer.h"
#include "DirectAudio.h"

using namespace Anubis;

DirectSoundAudioBuffer::DirectSoundAudioBuffer(LPDIRECTSOUNDBUFFER sample, shared_ptr<ResHandle> pResource)
	: AudioBuffer(pResource)
{
	m_Sample = sample;
	FillBufferWithSound();
}

APVOID DirectSoundAudioBuffer::VGet()
{
	if (!VRestore())
		return nullptr;

	return m_Sample;
}

ABOOL DirectSoundAudioBuffer::VPlay(AINT32 volume, ABOOL looping)
{
	if (!g_pAudio->VActive())
		return false;

	VStop();
	m_iVolume = volume;
	m_bLooping = looping;

	LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)VGet();
	if (!pDSB)
		return false;

	pDSB->SetVolume(volume);

	AUINT32 u32Flags = looping ? DSBPLAY_LOOPING : 0L;

	return (S_OK == pDSB->Play(0, 0, u32Flags));
}

ABOOL DirectSoundAudioBuffer::VStop()
{
	if (!g_pAudio->VActive())
		return false;

	LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)VGet();
	if (!pDSB)
		return false;

	m_bPaused = true;
	pDSB->Stop();

	return true;
}

ABOOL DirectSoundAudioBuffer::VResume()
{
	m_bPaused = false;
	return VPlay(VGetVolume(), VIsLooping());
}

ABOOL DirectSoundAudioBuffer::VTogglePause()
{
	if (!g_pAudio->VActive())
		return false;

	if (m_bPaused)
	{
		VResume();
	}
	else
	{
		VPause();
	}

	return true;
}

ABOOL DirectSoundAudioBuffer::VIsPlaying()
{
	if (!g_pAudio->VActive())
		return false;

	DWORD u32Status = 0;
	LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)VGet();
	pDSB->GetStatus(&u32Status);
	ABOOL bIsPlaying = ((u32Status & DSBSTATUS_PLAYING) != 0);

	return bIsPlaying;
}

AVOID DirectSoundAudioBuffer::VSetVolume(AINT32 volume)
{
	if (!g_pAudio->VActive())
		return;

	LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)VGet();
	assert(volume >= 0 && volume <= 100 && "Volume must be in range [0..100]");

	//from [0..100] to directX scale using logarithmic scale!
	AREAL coeff = (AREAL)volume / 100.0f;
	AREAL log = coeff > 0.1f ? 1+log10(coeff) : 0;
	AREAL range = (DSBVOLUME_MAX - DSBVOLUME_MIN);
	AREAL r32Volume = (range * log) + DSBVOLUME_MIN;

	assert(r32Volume >= DSBVOLUME_MIN && r32Volume <= DSBVOLUME_MAX && "Volume is not in range of DirectX scale!");
	HRESULT hr = pDSB->SetVolume(LONG(r32Volume));
	assert(hr == S_OK && "Error setting volume");
}

ABOOL DirectSoundAudioBuffer::VRestore()
{
	HRESULT hr;
	ABOOL bRestored;

	//restore buffer if it was lost
	if (FAILED(hr = RestoreBuffer(&bRestored)))
		return NULL;

	if (bRestored)
	{
		//fill buffer with new data
		if (FAILED(hr = FillBufferWithSound()))
			return NULL;
	}

	return true;
}

HRESULT DirectSoundAudioBuffer::RestoreBuffer(ABOOL* pbWasRestored)
{
	HRESULT hr;

	if (!m_Sample)
		return CO_E_NOTINITIALIZED;
	if (pbWasRestored)
		*pbWasRestored = false;

	DWORD u32Status;
	if (FAILED(hr = m_Sample->GetStatus(&u32Status)))
		return E_FAIL;

	if (u32Status & DSBSTATUS_BUFFERLOST)
	{
		AINT32 count = 0;
		do
		{
			hr = m_Sample->Restore();
			if (hr == DSERR_BUFFERLOST)
				Sleep(10);
		}
		while( (hr == m_Sample->Restore()) == DSERR_BUFFERLOST && ++count < 100);

		if (pbWasRestored != NULL)
			*pbWasRestored = true;

		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT DirectSoundAudioBuffer::FillBufferWithSound()
{
	HRESULT hr;
	APVOID pDSLockedBuffer = nullptr; //DirectSound buffer
	DWORD u32DSLockedBufferSize = 0; //size of directSound buffer
	DWORD u32WavDataRead = 0; //data to read from the wav
	if (!m_Sample)
		return CO_E_NOTINITIALIZED;

	//ensure we have a focus
	if (FAILED(hr = RestoreBuffer(NULL)))
		return E_FAIL;

	//???!!!AINT32 pcmBufferSize = m
	AINT32 pcmBufferSize = 0;
	//Lock the buffer
	if (FAILED(hr = m_Sample->Lock(0, pcmBufferSize, &pDSLockedBuffer, &u32DSLockedBufferSize, NULL, NULL, 0L)))
		return E_FAIL;

	if (pcmBufferSize == 0)
	{
		//fill with silence
		FillMemory((BYTE*)pDSLockedBuffer, u32DSLockedBufferSize, (BYTE)128);
	}
	else
	{
//???!!!		CopyMemory(pDSLockedBuffer, pointer, size);
		if (pcmBufferSize < (AINT32)u32DSLockedBufferSize)
		{
			//if the buffers' sizes are different fill the rest with silence
			FillMemory((BYTE*)pDSLockedBuffer + pcmBufferSize, u32DSLockedBufferSize - pcmBufferSize, (BYTE)128);
		}
	}

	//unlock the buffer
	m_Sample->Unlock(pDSLockedBuffer, u32DSLockedBufferSize, NULL, 0);

	return S_OK;
}

AREAL DirectSoundAudioBuffer::VGetProgress()
{
	LPDIRECTSOUNDBUFFER pDSB = (LPDIRECTSOUNDBUFFER)VGet();
	DWORD progress = 0;

	pDSB->GetCurrentPosition(&progress, NULL);
	AREAL length = (AREAL)m_pResource->Size();
	return (AREAL)progress / length;
}