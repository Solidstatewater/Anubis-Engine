#pragma once
//========================================================================
// WAVE.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

namespace Anubis
{
	class WaveResourceLoader : public IResourceLoader
	{
	public:

		//constructor
		WaveResourceLoader();

		AVIRTUAL ABOOL VUseRawFile() { return false; }
		AVIRTUAL ABOOL VNeedFile() { return false; }
		AVIRTUAL ABOOL VDiscardRawBufferAfterLoad() { return true; }
		AVIRTUAL AUINT32 VGetLoadedResourceSize(ACHAR *pRawBuffer, AUINT32 uRawSize);
		AVIRTUAL ABOOL VLoadResource(ACHAR *pRawBuffer, AUINT32 uRawSize, std::shared_ptr<ResHandle> pHandle);
		AVIRTUAL ASTRING VGetPattern() { return "*.wav"; }

	protected:
		ABOOL ParseWave(ACHAR *pWavStream, size_t length, std::shared_ptr<ResHandle> pHandle);
	};
};