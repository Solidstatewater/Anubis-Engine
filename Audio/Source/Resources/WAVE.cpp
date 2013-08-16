#include "Audio_pch.h"
#include "WAVE.h"
#include "../Audio.h"

using namespace Anubis;
using namespace std;

WaveResourceLoader::WaveResourceLoader()
{
}

AUINT32 WaveResourceLoader::VGetLoadedResourceSize(ACHAR *pRawBuffer, AUINT32 uRawSize)
{
	DWORD		file = 0; 
	DWORD		fileEnd = 0; 
	
	DWORD		length = 0;     
	DWORD		type = 0;									

	DWORD pos = 0;

	// mmioFOURCC -- converts four chars into a 4 byte integer code.
	// The first 4 bytes of a valid .wav file is 'R','I','F','F'

	type = *((DWORD *)(pRawBuffer+pos));		pos+=sizeof(DWORD);
	if(type != mmioFOURCC('R', 'I', 'F', 'F'))
		return false;	
	
	length = *((DWORD *)(pRawBuffer+pos));	pos+=sizeof(DWORD);
	type = *((DWORD *)(pRawBuffer+pos));		pos+=sizeof(DWORD);

	// 'W','A','V','E' for a legal .wav file
	if(type != mmioFOURCC('W', 'A', 'V', 'E'))
		return false;		//not a WAV

	// Find the end of the file
	fileEnd = length - 4;
	
	bool copiedBuffer = false;

	// Load the .wav format and the .wav data
	// Note that these blocks can be in either order.
	while(file < fileEnd)
	{
		type = *((DWORD *)(pRawBuffer+pos));		pos+=sizeof(DWORD);
		file += sizeof(DWORD);

		length = *((DWORD *)(pRawBuffer+pos));	pos+=sizeof(DWORD);
		file += sizeof(DWORD);

		switch(type)
		{
			case mmioFOURCC('f', 'a', 'c', 't'):
			{
				assert(false && "This wav file is compressed.  We don't handle compressed wav at this time");
				break;
			}

			case mmioFOURCC('f', 'm', 't', ' '):
			{
				pos+=length;   
				break;
			}

			case mmioFOURCC('d', 'a', 't', 'a'):
			{
				return length;
			}
		} 

		file += length;

		// Increment the pointer past the block we just read,
		// and make sure the pointer is word aliged.
		if (length & 1)
		{
			++pos;
			++file;
		}
	} 

	// If we get to here, the .wav file didn't contain all the right pieces.
	return false; 
}

ABOOL WaveResourceLoader::VLoadResource(char *pRawBuffer, unsigned int rawSize, shared_ptr<ResHandle> pHandle)
{
	std::shared_ptr<SoundResourceExtraData> extra = std::shared_ptr<SoundResourceExtraData>(new SoundResourceExtraData());
	extra->m_SoundType = ST_wave;
	pHandle->SetExtra(std::shared_ptr<SoundResourceExtraData>(extra));
	if (!ParseWave(pRawBuffer, rawSize, pHandle))
	{
		return false;
	}
	return true;
}

ABOOL WaveResourceLoader::ParseWave(ACHAR *pWavStream, size_t bufferLength, shared_ptr<ResHandle> pHandle)
{
	std::shared_ptr<SoundResourceExtraData> extra = std::static_pointer_cast<SoundResourceExtraData>(pHandle->GetExtra());
	DWORD		file = 0; 
	DWORD		fileEnd = 0; 
	
	DWORD		length = 0;     
	DWORD		type = 0;									

	DWORD pos = 0;

	// mmioFOURCC -- converts four chars into a 4 byte integer code.
	// The first 4 bytes of a valid .wav file is 'R','I','F','F'

	type = *((DWORD *)(pWavStream+pos));		pos+=sizeof(DWORD);
	if(type != mmioFOURCC('R', 'I', 'F', 'F'))
		return false;	
	
	length = *((DWORD *)(pWavStream+pos));	pos+=sizeof(DWORD);
	type = *((DWORD *)(pWavStream+pos));		pos+=sizeof(DWORD);

	// 'W','A','V','E' for a legal .wav file
	if(type != mmioFOURCC('W', 'A', 'V', 'E'))
		return false;		//not a WAV

	// Find the end of the file
	fileEnd = length - 4;
	
	memset(&extra->m_WavFormatEx, 0, sizeof(WAVEFORMATEX));

	bool copiedBuffer = false;

	// Load the .wav format and the .wav data
	// Note that these blocks can be in either order.
	while(file < fileEnd)
	{
		type = *((DWORD *)(pWavStream+pos));		pos+=sizeof(DWORD);
		file += sizeof(DWORD);

		length = *((DWORD *)(pWavStream+pos));	pos+=sizeof(DWORD);
		file += sizeof(DWORD);

		switch(type)
		{
			case mmioFOURCC('f', 'a', 'c', 't'):
			{
				assert(false && "This wav file is compressed.  We don't handle compressed wav at this time");
				break;
			}

			case mmioFOURCC('f', 'm', 't', ' '):
			{
				memcpy(&extra->m_WavFormatEx, pWavStream+pos, length);		pos+=length;   
				extra->m_WavFormatEx.cbSize = (WORD)length;
				break;
			}

			case mmioFOURCC('d', 'a', 't', 'a'):
			{
				copiedBuffer = true;
				if (length != pHandle->Size())
				{
					assert(0 && "Wav resource size does not equal the buffer size");
					return 0;
				}
				memcpy(pHandle->WritableBuffer(), pWavStream+pos, length);			pos+=length;
				break;
			}
		} 

		file += length;

		// If both blocks have been seen, we can return true.
		if( copiedBuffer )
		{
			extra->m_LengthMilli = ( pHandle->Size() * 1000 ) / extra->GetFormat()->nAvgBytesPerSec;
			return true;
		}

		// Increment the pointer past the block we just read,
		// and make sure the pointer is word aliged.
		if (length & 1)
		{
			++pos;
			++file;
		}
	} 

	// If we get to here, the .wav file didn't contain all the right pieces.
	return false; 
}
