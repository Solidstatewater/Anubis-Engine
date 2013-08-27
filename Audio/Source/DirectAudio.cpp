//====================================================================================
//DirectAudio.cpp
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
#include "DirectAudio.h"
#include "DirectAudioBuffer.h"

using namespace Anubis;

ABOOL DirectSoundAudio::VInitialize(HWND hWnd)
{
	if (m_bInitialized)
		return true;
	m_bInitialized = false;
	m_AllSamples.clear();

	SAFE_RELEASE(m_pDS);

	HRESULT hr;

	//now create IDirectSound using primary sound device
	if (FAILED(hr = DirectSoundCreate8(NULL, &m_pDS, NULL)))
		return false;

	//set cooperative level for DirectSound
	//use DSSCL_PRIORITY to have control of struture of sound data
	if (FAILED(hr = m_pDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return false;

	//set primary buffer format
	//8 channels, 44Khz, 16-bit
	if (FAILED(hr = SetPrimaryBufferFormat(8, 44100, 16)))
		return false;

	//now everything is initialized
	m_bInitialized = true;

	return true;
}

HRESULT DirectSoundAudio::SetPrimaryBufferFormat(
	AUINT32 u32PrimaryChannels,
	AUINT32 u32PrimaryFreq,
	AUINT32 u32PrimaryBitRate )
{
	HRESULT hr;
	LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

	if (!m_pDS)
		return CO_E_NOTINITIALIZED;

	//get the primary buffer
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL)))
		return E_FAIL;

	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = (WORD)WAVE_FORMAT_PCM;
	wfx.nChannels = (WORD)u32PrimaryChannels;
	wfx.nSamplesPerSec = (DWORD)u32PrimaryFreq;
	wfx.wBitsPerSample = (WORD)u32PrimaryBitRate;
	wfx.nBlockAlign = (WORD)(wfx.wBitsPerSample / 8 * wfx.nChannels);
	wfx.nAvgBytesPerSec = (DWORD)(wfx.nSamplesPerSec * wfx.nBlockAlign);

	if (FAILED(hr = pDSBPrimary->SetFormat(&wfx)))
		return E_FAIL;

	SAFE_RELEASE(pDSBPrimary);

	return S_OK;
}

AVOID DirectSoundAudio::VShutDown()
{
	if (m_bInitialized)
	{
		Audio::VShutDown();
		SAFE_RELEASE(m_pDS);
		m_bInitialized = false;
	}
}

IAudioBuffer* DirectSoundAudio::VInitAudioBuffer(shared_ptr<ResHandle> pHandle)
{
	std::shared_ptr<SoundResourceExtraData> extra = static_pointer_cast<SoundResourceExtraData>(pHandle->GetExtra());

	if (!m_pDS)
		return nullptr;

	switch(extra->GetSoundType())
	{
		case ST_wave:
			break;

		default:
			assert(false && "Unsupported sound type");
			return NULL;
	}//end switch

	LPDIRECTSOUNDBUFFER sampleHandle;
	//create direcct sound buffer
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = pHandle->Size();
	dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat = const_cast<WAVEFORMATEX*>(extra->GetFormat());

	HRESULT hr;
	if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &sampleHandle, NULL)))
		return NULL;

	//add handle to the list
	IAudioBuffer *pBuffer = (IAudioBuffer*)(new DirectSoundAudioBuffer(sampleHandle, pHandle));
	m_AllSamples.push_front(pBuffer);

	return pBuffer;
}

AVOID DirectSoundAudio::VReleaseAudioBuffer(IAudioBuffer *pSampleHandle)
{
	pSampleHandle->VStop();
	m_AllSamples.remove(pSampleHandle);
}