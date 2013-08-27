//====================================================================================
//JSONLoader.cpp : Implement JSON-file reader
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
#include "FileStream_pch.h"
#include "JSONLoader.h"
#include "rapidjson\FileStream.h"

#include <fstream>

using namespace Anubis;
using namespace rapidjson;
using namespace std;

GenericDocument<struct rapidjson::UTF8<char>, class rapidjson::MemoryPoolAllocator<class rapidjson::CrtAllocator> > g_doc;
rapidjson::Value g_array;

/*** Parse document	***/
ABOOL JSONLoader::Parse(const ACHAR * const pJSON)
{
	if (!pJSON)
	{
		assert(0 && "Null pointer");
		return false;
	}

	//parse file
	g_doc.Parse<0>(pJSON);

	return true;
}
ABOOL JSONLoader::Parse(const ASTRING & filename)
{
	//try to open the file first
	FILE* pFile = new FILE();
	fopen_s(&pFile, filename.c_str(), "r");
	if (pFile == NULL)
	{
		assert(0 && "Couldn't open the file");
		return false;
	}

	//try to parse it
	FileStream file(pFile);
	g_doc.ParseStream<0>(file);

	return true;
}

ABOOL	JSONLoader::GetBool		(const ACHAR * const field)
{
	assert(g_doc[field].IsBool() && "Field is not BOOL type!!!");

	return g_doc[field].IsTrue();
}

ASTRING	JSONLoader::GetString	(const ACHAR * const field)
{
	assert(g_doc[field].IsString() && "Field is not string type!!!");

	return g_doc[field].GetString();
}

AUINT32	JSONLoader::GetAUINT32	(const ACHAR * const field)
{
	assert(g_doc[field].IsUint() && "Field is not AUINT32 type!!!");

	return g_doc[field].GetUint();
}

AUINT64	JSONLoader::GetAUINT64	(const ACHAR * const field)
{
	assert(g_doc[field].IsUint64() && "Field is not AUINT64 type!!!");

	return g_doc[field].GetUint64();
}

AINT32	JSONLoader::GetAINT32	(const ACHAR * const field)
{
	assert(g_doc[field].IsInt() && "Field is not AINT32 type!!!");

	return g_doc[field].GetInt();
}

AINT64	JSONLoader::GetAINT64	(const ACHAR * const field)
{
	assert(g_doc[field].IsInt64() && "Field is not AINT64 type!!!");

	return g_doc[field].GetInt64();
}

AREAL32	JSONLoader::GetAREAL32	(const ACHAR * const field)
{
	assert(g_doc[field].IsDouble() && "Field is not AREAl32 type!!!");

	return static_cast<AREAL32>(g_doc[field].GetDouble());
}

AREAL64	JSONLoader::GetAREAL64	(const ACHAR * const field)
{
	assert(g_doc[field].IsDouble() && "Field is not AREAL64 type!!!");

	return g_doc[field].GetDouble();
}

///////////////////////////////////////////
//
//	Parse Arrays
//
///////////////////////////////////////////
AVOID JSONLoader::ParseArray	(const ACHAR * const field)
{
	g_array = g_doc[field];

	assert(g_array.IsArray() && "Field with this name is not array!");
}

AUINT32 JSONLoader::ArraySize()
{
	return g_array.Size();
}

ABOOL	JSONLoader::GetBoolFromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].IsTrue();
	else
		return g_array[index].IsTrue();
}
ASTRING	JSONLoader::GetStringFromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].GetString();
	else
		return g_array[index].GetString();
}
AUINT32	JSONLoader::GetAUINT32FromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].GetUint();
	else
		return g_array[index].GetUint();
}
AUINT64	JSONLoader::GetAUINT64FromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].GetUint64();
	else
		return g_array[index].GetUint64();
}
AINT32	JSONLoader::GetAINT32FromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].GetInt();
	else
		return g_array[index].GetInt();
}
AINT64	JSONLoader::GetAINT64FromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].GetInt64();
	else
		return g_array[index].GetInt64();
}
AREAL32	JSONLoader::GetAREAL32FromArray	(const AINT32 index)
{
	if (!index)
		return static_cast<AREAL32>(g_array[0u].GetDouble());
	else
		return static_cast<AREAL32>(g_array[index].GetDouble());
}
AREAL64	JSONLoader::GetAREAL64FromArray	(const AINT32 index)
{
	if (!index)
		return g_array[0u].GetDouble();
	else
		return g_array[index].GetDouble();
}