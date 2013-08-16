//====================================================================================
//ResourceFile.cpp
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

#include "ResourceManager_pch.h"
#include "ResourceFile.h"

#include <ctype.h>

using namespace Anubis;

ResourceDir::ResourceDir(const AWSTRING & resourceDir)
{
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	m_directory = dir;
	AINT32 iLastSlashPos = m_directory.find_last_of(L"\\");
	//m_directory = m_directory.substr(0, iLastSlashPos);
	m_directory += L"\\Resources\\";
}

ABOOL ResourceDir::VOpen()
{
	ReadResourceDirectory(L"*");
	return true;
}

AVOID ResourceDir::ReadResourceDirectory(const AWSTRING & pattern)
{
	HANDLE pHandle;
	WIN32_FIND_DATA data;

	AWSTRING path = m_directory + pattern;
	pHandle = FindFirstFile(path.c_str(), &data);
	if (pHandle != INVALID_HANDLE_VALUE)
	{
		//iterate through all files left in directory
		while(FindNextFile(pHandle, &data))
		{
			if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			AWSTRING fileName = data.cFileName;
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = pattern.substr(0, pattern.length()-1) + fileName + L"\\*";
					ReadResourceDirectory(fileName);
				}
			}
			else
			{
				fileName = pattern.substr(0, pattern.length()-1) + fileName;
				AWSTRING lower = fileName;
				//std::transform(lower.begin(), lower.end(), lower.begin(), std::tolower);
				//std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower<int>);
				wcscpy_s(&data.cFileName[0], MAX_PATH, lower.c_str());
				m_directoryContent[ws2s(lower)] = m_resourcesData.size();
				m_resourcesData.push_back(data);
			}
		}
	}

	FindClose(pHandle);
}

AINT32 ResourceDir::Find(const ASTRING & name)
{
	ASTRING lower = name;
	//std::transform(lower.begin(), lower.end(), lower.begin(), (AINT32(*)(AINT32)) std::tolower<AINT32>);
	ContentMap::const_iterator it = m_directoryContent.find(lower);
	
	if (it != m_directoryContent.end())
	{
		return it->second;
	}

	//no such resource in the directory
	return -1;
}

AINT32 ResourceDir::VGetRawResourceSize(const Resource & resource)
{
	AINT32 iNum = Find(resource.m_name.c_str());
	if (iNum == -1)
		return -1;

	return m_resourcesData[iNum].nFileSizeLow;
}

AINT32 ResourceDir::VGetRawResource(const Resource & resource, ACHAR* pBuffer)
{
	AINT32 iNum = Find(resource.m_name.c_str());
	if (iNum == -1)
		return -1;

	ASTRING fullFileSpec = ws2s(m_directory) + resource.m_name.c_str();
	FILE* pFile = fopen(fullFileSpec.c_str(), "rb");
	size_t bytes = fread(pBuffer, 1, m_resourcesData[iNum].nFileSizeLow, pFile);
	fclose(pFile);

	return bytes;
}

AINT32 ResourceDir::VGetNumResources() const
{
	return m_resourcesData.size();
}

ASTRING ResourceDir::VGetResourceName(const AINT32 num) const
{
	AWSTRING resName = m_resourcesData[num].cFileName;
	return ws2s(resName);
}