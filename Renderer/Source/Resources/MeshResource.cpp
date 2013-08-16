//====================================================================================
//MeshResource.cpp
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

#include "Renderer_pch.h"
#include "MeshResource.h"

#include "../../Math/Source/Vectors.h"
#include "../../Math/Source/Quaternion.h"

#include "Final\Resources\Buffers\VertexBuffer.h"
#include "Final\Resources\Buffers\IndexBuffer.h"
#include "Final\Resources\Buffers\BufferParams.h"
#include "Final\\Resources\ResourceData.h"
#include "../Meshes/Meshes.h"

using namespace Anubis;
using namespace std;

////////////////////////////////////////////
//Assimp conversion methods
////////////////////////////////////////////
float3 aiVector3D_to_float3(aiVector3D v)
{
	return float3(v.x, v.y, v.z);
}

float2 aiVector3D_to_float2(aiVector3D v)
{
	return float2(v.x, v.y);
}

Vec aiColor4D_to_Vec(aiColor4D c)
{
	return Vector(c.r, c.g, c.b, c.a);
}

Quaternion aiQuaternion_to_Quaternion(aiQuaternion q)
{
	return Quaternion(q.x, q.y, q.z, q.w);
}

Mat4x4 aiMatrix4x4_to_Mat4x4(aiMatrix4x4 mat)
{
	Mat4x4 res;

	for (AINT8 i = 0; i < 4; i++)
	{
		for (AINT8 j = 0; j < 4; j++)
		{
			if (j == 0)
				res.rows[i].x = mat[i][j];
			else if (j == 1)
				res.rows[i].y = mat[i][j];
			else if (j == 2)
				res.rows[i].z = mat[i][j];
			else if (j == 3)
				res.rows[i].w = mat[i][j];
		}
	}

	return res;
}


MeshResourceExtraData::MeshResourceExtraData()
{
	m_uNumMeshes = 0;
}

ObjResourceLoader::ObjResourceLoader()
{
}

AUINT32 ObjResourceLoader::VGetLoadedResourceSize(ACHAR *pRawBuffer, AUINT32 uRawSize)
{
	return 10; 
}

ABOOL ObjResourceLoader::VLoadResource(ACHAR *pRawBuffer, unsigned int uRawSize, shared_ptr<ResHandle> pHandle)
{
	//std::shared_ptr<SoundResourceExtraData> extra = std::shared_ptr<SoundResourceExtraData>(new SoundResourceExtraData());
	//extra->m_SoundType = SOUND_TYPE_WAVE;
	//pHandle->SetExtra(std::shared_ptr<SoundResourceExtraData>(extra));
	//if (!ParseWave(pRawBuffer, rawSize, pHandle))
	//{
	//	return false;
	//}
	//return true;
	std::shared_ptr<MeshResourceExtraData> pExtra = std::shared_ptr<MeshResourceExtraData>(new MeshResourceExtraData());
	pHandle->SetExtra(pExtra);
	if (!ParseObj(pRawBuffer, uRawSize, pHandle))
	{
		return false;
	}
	return true;
}

/*ABOOL ObjResourceLoader::LoadMtl(NameMaterialMap & map, AWSTRING fileName)
{
	wifstream file(fileName.c_str());
	WCHAR ch;
	//temporary pointer for material
	MaterialPtr pMaterial;
	std::wstring textureFileName; //name of file containing texture

	if (!file) return false;

	std::wstring materialName;

	while (file)
	{
		ch = file.get();

		switch (ch)
		{
			//skip comment line
			case '#':
				while ((ch = file.get()) != '\n');
				break;

			case 'n':
				//get 'newmtl' keyword
				if ((ch = file.get()) != 'e') break;
				if ((ch = file.get()) != 'w') break;
				if ((ch = file.get()) != 'm') break;
				if ((ch = file.get()) != 't') break;
				if ((ch = file.get()) != 'l') break;
				if ((ch = file.get()) != ' ') break;

				//save previous material data
				if (pMaterial)
					map.insert(std::make_pair(materialName, pMaterial));

				//get material name
				file >> materialName;

				//reset material ptr as we filling new one
				pMaterial.reset(GCC_NEW Material());

				//go to the next line
				while ((ch = file.get()) != '\n');
				break;

			case 'N':
				//check the next character
				ch = file.get();

				//get specular power
				if (ch == 's')
				{
					file >> (pMaterial->m_specularPower);
					break;
				}

				while ((ch = file.get()) != '\n');

				break;

			case 'd':
				float trans;
				file >> trans; //read transparency 
				//set material alpha channel
				pMaterial->SetAlpha(trans);
				break;

			case 'T':
				ch = file.get();
				//inverse transparency
				if (ch == 'r')
				{
					float invsTrans;
					file >> invsTrans;
					invsTrans = 1.0f - invsTrans;

					pMaterial->SetAlpha(invsTrans);
				}
				break;

			case 'K':
				ch = file.get();

				//ambient color
				if (ch == 'a')
				{
					file >> pMaterial->m_ambientColor.x;
					file >> pMaterial->m_ambientColor.y;
					file >> pMaterial->m_ambientColor.z;
					break;
				}

				//diffuse color
				if (ch == 'd')
				{
					file >> pMaterial->m_diffuseColor.x;
					file >> pMaterial->m_diffuseColor.y;
					file >> pMaterial->m_diffuseColor.z;
					break;
				}

				//specular color
				if (ch == 's')
				{
					file >> pMaterial->m_specularColor.x;
					file >> pMaterial->m_specularColor.y;
					file >> pMaterial->m_specularColor.z;
					break;
				}

				//emissive color
				if (ch == 'e')
				{
					file >> pMaterial->m_emissiveColor.x;
					file >> pMaterial->m_emissiveColor.y;
					file >> pMaterial->m_emissiveColor.z;
					break;
				}

				break;

			case 'm':
				if ((ch = file.get()) != 'a') break;
				if ((ch = file.get()) != 'p') break;
				if ((ch = file.get()) != '_') break;

				//so, we gonna load some texture...
				ch = file.get();
				if (ch == 'K')
				{
					ch = file.get();
					//ambient texture
					if (ch == 'a')
					{
						if ((ch = file.get() != ' ')) break;
						textureFileName = L"";
						while ((ch = file.get()) != '\n') 
						{
							textureFileName += ch;
						}
						pMaterial->m_pAmbientTexture->CreateFromFile(textureFileName.c_str());
						break;
					}
					//diffuse
					if (ch == 'd')
					{
						if ((ch = file.get() != ' ')) break;
						textureFileName = L"";
						while ((ch = file.get()) != '\n') 
						{
							textureFileName += ch;
						}
						pMaterial->m_pDiffuseTexture->CreateFromFile(textureFileName.c_str());
						break;
					}
					//specular map
					if (ch == 's')
					{
						if ((ch = file.get() != ' ')) break;
						textureFileName = L"";
						while ((ch = file.get()) != '\n') 
						{
							textureFileName += ch;
						}
						pMaterial->m_pSpecularMap->CreateFromFile(textureFileName.c_str());
						break;
					}
				}

				if (ch == 'b')
				{
					if ((ch = file.get()) != 'u') break;
					if ((ch = file.get()) != 'm') break;
					if ((ch = file.get()) != 'p') break;
					if ((ch = file.get()) != ' ') break;

					//load bump map
					textureFileName = L"";
					while ((ch = file.get()) != '\n' && ch != 65535) 
					{
						textureFileName += ch;
					}
					pMaterial->m_pBumpMap->CreateFromFile(textureFileName.c_str());
					break;
				}

				break;

			case 'b':
				if ((ch = file.get()) != 'u') break;
				if ((ch = file.get()) != 'm') break;
				if ((ch = file.get()) != 'p') break;
				if ((ch = file.get()) != ' ') break;

				//load bump map
				textureFileName = L"";
				while ((ch = file.get()) != '\n' || file.eof()) 
				{
					textureFileName += ch;
				}
				pMaterial->m_pBumpMap->CreateFromFile(textureFileName.c_str());
				break;
		}
	}

	//add last material to the list
	map.insert(std::make_pair(materialName, pMaterial));

	return true;
}
*/

AVOID UnsupportedtoJPG(ACHAR * pStr)
{
	ACHAR * pSep = const_cast<ACHAR*>(strchr(pStr, '.'));

	if (!pSep)	return;

	if (!strcmp(pSep, ".tga"))
		strcpy(pSep, ".jpg");
}

ABOOL ObjResourceLoader::ParseObj(ACHAR *pObjStream, size_t bufferLength, shared_ptr<ResHandle> pHandle)
{
	std::shared_ptr<MeshResourceExtraData> pExtra = std::static_pointer_cast<MeshResourceExtraData>(pHandle->GetExtra());
	//VertexBuffer** ppVerticesBuffers	= &pExtra->GetPositionData();
	//VertexBuffer** ppNormalBuffers		= &pExtra->GetNormalData();
	//VertexBuffer** ppTexCoordBuffers	= &pExtra->GetTexCoordData();
	//IndexBuffer**  ppIndexBuffers		= &pExtra->GetIndexData();
/*
#pragma region Data_Declaration
	wifstream file(fileName.c_str());
	WCHAR ch;

	if (!file) return E_FAIL;

	//Declare temporary variables and lists
	AWSTRING mtllibName;
	AWSTRING materialName;
	ABOOL bRightHanded = true;
	std::vector<float3> positionList;
	std::vector<float2> texCoordList;
	std::vector<float3> normalList;
	std::vector<DWORD> groupIndices;
	std::map<AINT32, AWSTRING> indexMaterialMap; //maps start index to material name
	DWORD curVertex = 0x00;

	struct Vertex
	{
		Vertex(DWORD pos, DWORD texCoord, DWORD normal)
			: posIndex(pos), texCoordIndex(texCoord), normalIndex(normal) {}
		~Vertex() {}

		DWORD posIndex;
		DWORD texCoordIndex;
		DWORD normalIndex;
	};

	std::vector<Vertex> vertexList;
	std::vector<DWORD> indexList;

	AUINT32 indexCount = 0;
	#pragma endregion

#pragma region Helper_Class
	//helper class for extracting face data
	static class Helper
	{
	public:
		static void GetVertexData(AWSTRING vertexDef,
			DWORD & posIndex, DWORD & texCoordIndex, DWORD & normalIndex)
		{
			AUINT32 index = 0;
			AWSTRING tempData = L"";

			//get position data
			while (vertexDef[index] != '/')
			{
				tempData += vertexDef[index];
				index++;
				if (index == vertexDef.length()) break;
			}

			std::wistringstream idata(tempData);
			idata >> posIndex;
			posIndex--;

			if (index == vertexDef.length())
			{
				texCoordIndex = 0;
				normalIndex = 0;
				return; //only position data
			}

			index++;
			if (vertexDef[index] == '/') //next is normal data
			{
				index++;
				tempData = L"";
				while (index != vertexDef.length())
				{
					tempData += vertexDef[index];
					index++;
				}

				std::wistringstream idata(tempData);
				idata >> normalIndex;
				normalIndex--;

				return;
			}
			else //next is texture data
			{
				tempData = L"";
				while (vertexDef[index] != '/')
				{
					tempData += vertexDef[index];
					index++;
					if (index == vertexDef.length()) break;
				}

				std::wistringstream idata(tempData);
				idata >> texCoordIndex;
				texCoordIndex--;

				if (index == vertexDef.length()) return;
				else //read normal
				{
					index++;
					tempData = L"";
					while (index != vertexDef.length())
					{
						tempData += vertexDef[index];
						index++;
					}

					std::wistringstream idata(tempData);
					idata >> normalIndex;
					normalIndex--;
				}
			}
		}
		static DWORD UniqueIndexCount(DWORD* indices, DWORD indexCount) //return number of unique indices
		{
			DWORD unique = 0;
			DWORD* uniqueList = new DWORD[indexCount];

			for (int i = 0; i < indexCount; i++)
			{
				ABOOL bExists = false;

				for (int j = 0; j < unique; j++)
				{
					if (uniqueList[j] == indices[i])
					{
						bExists = true;
						break;
					}
				}

				//we already have this index
				if (bExists) continue;

				uniqueList[unique++] = indices[i];
			}

			return unique;
		}
	};
#pragma endregion

#pragma region Parsing
	//iterate unlit we reach end of file
	while (file)
	{
		ch = file.get();

		switch (ch)
		{
		case '#': //comment string
			while (ch != '\n') ch = file.get();
			break;
		case 'v': //vertex data
			ch = file.get();
			if (ch == ' ') //vertex position
			{
				float3 pos;
				file >> pos.x >> pos.y >> pos.z;
				if (bRightHanded) pos.z *= -1.0f;

				//add position to the list
				positionList.push_back(pos);
			}
			if (ch == 't') //vertex texture coordinates
			{
				float2 texCoord; float temp;
				file >> texCoord.x >> texCoord.y;
				file >> temp; //third coordinate is unused
				if (bRightHanded) texCoord.y = 1.0f - texCoord.y;

				//add texture coordinate to the list
				texCoordList.push_back(texCoord);
			}
			if (ch == 'n') //vertex normal coordinates
			{
				float3 normal;
				file >> normal.x >> normal.y >> normal.z;
				if (bRightHanded) normal.z *= -1.0f;

				//add normal to the list
				normalList.push_back(normal);
			}

			break;
		case 'g': //new group
			while ((ch = file.get()) != '\n'); // skip string
			groupIndices.push_back(indexCount);
			break;

		case 'm': //hope to find mtllib - name of material library
			if ((ch = file.get()) != 't') break;
			if ((ch = file.get()) != 'l') break;
			if ((ch = file.get()) != 'l') break;
			if ((ch = file.get()) != 'i') break;
			if ((ch = file.get()) != 'b') break;
			if ((ch = file.get()) != ' ') break;

			file >> mtllibName;
			break;

		case 'u': //hope to find usemtl - name of material library to use
			if ((ch = file.get()) != 's') break;
			if ((ch = file.get()) != 'e') break;
			if ((ch = file.get()) != 'm') break;
			if ((ch = file.get()) != 't') break;
			if ((ch = file.get()) != 'l') break;
			if ((ch = file.get()) != ' ') break;

			materialName = L"";
			file >> materialName;

			indexMaterialMap.insert(std::make_pair(indexCount, materialName));

			while ((ch = file.get()) != '\n');

			break;

		case 's': //smooth shading
			//skip it
			while ((ch = file.get()) != '\n');
			break;

		case 'f': //face definition
		{
			AUINT32 spaceCount = 0;
			AUINT32 triangleCount;
			AWSTRING face = L"";

			//temp data
			DWORD posIndex, texCoordIndex, normalIndex;
			posIndex = texCoordIndex = normalIndex = 0;
			
			while ((ch  = file.get()) != '\n') //loop until we reach end of string
			{
				if (ch == ' ') spaceCount++; //one more space
				face += ch;
			}

			if (face.empty()) break; 

			if (face[face.length() - 1] == ' ') spaceCount--; //don't count last space
			triangleCount = 1 + (spaceCount - 3);

			//cannot build a face without a single triangle
			if (triangleCount < 1)
			{
				assert(0 && "Corrupted face data!");
			}

			std::wstringstream str(face);
			AWSTRING vertexDef;

			DWORD firstIndex, lastIndex;
			firstIndex = curVertex;

			//create first triangle
			for (int i = 0; i < 3; i++)
			{
				str >> vertexDef;

				Helper::GetVertexData(vertexDef, posIndex, texCoordIndex, normalIndex);
				vertexList.push_back(Vertex(posIndex, texCoordIndex, normalIndex));
				indexList.push_back(curVertex);
				indexCount++;
				curVertex++;
			}

			lastIndex = curVertex - 1;

			//add other vertices to the face if there are any
			while ((triangleCount - 1) > 0)
			{
				int newIndex, i;
				newIndex = -1; i = 0;
				triangleCount--;

				//Add first two vertices of new triangle
				indexList.push_back(firstIndex);
				indexList.push_back(lastIndex);
				indexCount += 2;

				str >> vertexDef;

				Helper::GetVertexData(vertexDef, posIndex, texCoordIndex, normalIndex);
				//make sure not ot make duplicate vertex
				for (std::vector<Vertex>::iterator iter = vertexList.begin(); iter != vertexList.end(); iter++)
				{
					Vertex vertex = (*iter);
					if (vertex.posIndex == posIndex && vertex.texCoordIndex == texCoordIndex &&
						vertex.normalIndex == normalIndex)
					{
						newIndex = i;
						break;
					}
				}
				if (newIndex == -1)
				{
					lastIndex = curVertex;
					vertexList.push_back(Vertex(posIndex, texCoordIndex, normalIndex));
					indexList.push_back(curVertex);
					indexCount++;
					curVertex++;
				}
				else
				{
					indexList.push_back(newIndex);	
					indexCount++;
					lastIndex = newIndex;
				}
			}



			break;
		}

		case ' ':
			while ((ch = file.get()) != '\n');
			break;

		default: //not defined
			break;
		}
	}

	//put last end index to the list
	groupIndices.push_back(indexCount);
#pragma endregion

#pragma region Subset_Creation
	//firstly, load materials 
	NameMaterialMap materials;
	LoadMtl(materials, mtllibName);

	//initilize vertex and index buffers arrays
	numSubsets = groupIndices.size() - 1;
	//*ppVertexBuffers = new VertexBuffer[numSubsets];
	//*ppIndexBuffers = new IndexBuffer[numSubsets];
	*ppVerticesBuffers	= new VertexBuffer[numSubsets];
	*ppNormalBuffers	= new VertexBuffer[numSubsets];
	*ppTexCoordBuffers	= new VertexBuffer[numSubsets];
	*ppIndexBuffers		= new IndexBuffer[numSubsets];
	*ppMaterials = new MaterialKeeper[numSubsets];

	DWORD startIndex = 0;
	UINT curSubset = 0;

	//fill and create all vertex and index buffers
	vector<DWORD>::iterator iter = groupIndices.begin();
	iter++;
	for (; iter != groupIndices.end(); iter++)
	{
		//get final index of current subset
		DWORD endIndex = (*iter);
		DWORD numIndices = endIndex - startIndex;
		DWORD* indices = new DWORD[numIndices];

		//fill indices
		for (int i = startIndex; i < endIndex; i++)
		{
			indices[i - startIndex] = indexList[i];
		}

		//DWORD uniqueIndices = Helper::UniqueIndexCount(indices, numIndices);
		DWORD uniqueIndices = numIndices;

		DWORD addedVertices = 0;
		DWORD addedIndices = 0;
		UINT addedMaterials = 0;
		//Bump_Vertex* v = new Bump_Vertex[uniqueIndices];
		float3* posArray = new float3[uniqueIndices];
		float2* texCoordArray = new float2[uniqueIndices];
		NormalData* normalDataArray = new NormalData[uniqueIndices];

		BufferParams posBufferParams, texCoordBufferParams, normalBufferParams;
		posBufferParams.FillVertexBufferParams(sizeof(float3), uniqueIndices, true, false, false, false);
		texCoordBufferParams.FillVertexBufferParams(sizeof(float2), uniqueIndices, true, false, false, false);
		normalBufferParams.FillVertexBufferParams(sizeof(NormalData), uniqueIndices, true, false, false, false); 

		SubresourceData data;

		for (int i = startIndex; i < endIndex; i++)
		{
			//check whether we need new materials
			std::map<int, std::wstring>::iterator iter;
			if ((iter = indexMaterialMap.find(i)) != indexMaterialMap.end())
			{
				//set end index for current material
				if (addedMaterials > 0)
				{
					//materials[materialName]->endIndex = i-1;
					(*ppMaterials)[curSubset].materials[addedMaterials-1].endIndex = i-1;
				}

				materialName = (*iter).second;

				//find according material in the map
				NameMaterialMap::iterator mat = materials.find(materialName);

				//no material with such name found, there must be some error
				if (mat == materials.end())
				{
					assert(0 && "Error loading .obj! No such material found!");
				}

				//then replace zero when adding new material!!!!!
				//Material* newMat = &(*(*mat).second);
				(*ppMaterials)[curSubset].materials.push_back(MeshMaterialData((*mat).second, addedIndices, 0));
				addedMaterials++;
			}

			Vertex vertexData = vertexList[indexList[i] ];

			//check whether we already have the same vertex in the array
			bool bExists = false;
			//for (int j = 0; j < addedVertices; j++)
			//{
			//	if (v[i].pos == positionList[vertexData.posIndex] &&
			//		v[i].texCoord == texCoordList[vertexData.texCoordIndex] &&
			//		v[i].normal == normalList[vertexData.normalIndex])
			//	{
			//		bExists = true;
			//		indices[addedIndices] == j;
			//		addedIndices++;
			//		break;
			//	}
			//} 

			if (!bExists)
			{
				indices[addedIndices] = addedVertices;
				addedIndices++;
			}

			//new vertex
			if (!bExists)
			{
				if (addedVertices == 526)
				{
					int i;
					i = 3+4;
					i = 5;
				}

				float posX, posY, posZ, texU, texV, normalX, normalY, normalZ;
				posX = positionList[vertexData.posIndex].x;
				posY = positionList[vertexData.posIndex].y;
				posZ = positionList[vertexData.posIndex].z;
				texU = texCoordList[vertexData.texCoordIndex].x;
				texV = texCoordList[vertexData.texCoordIndex].y;
				normalX = normalList[vertexData.normalIndex].x;
				normalY = normalList[vertexData.normalIndex].y;
				normalZ = normalList[vertexData.normalIndex].z;


				//XMFLOAT3 pos(posX, posY, posZ);
				//XMFLOAT2 tex(texU, texV);
				//XMFLOAT3 normal(normalX, normalY, normalZ);
				float3 pos(posX, posY, posZ);
				float2 tex(texU, texV);
				float3 normal(normalX, normalY, normalZ);

				//Bump_Vertex vertex(posX, posY, posZ, texU, texV, normalX, normalY, normalZ);

				//v[addedVertices++] = vertex;
				posArray[addedVertices] = pos;
				texCoordArray[addedVertices] = tex;
				normalDataArray[addedVertices].normal = normal;
				addedVertices++;
			}
		}

		startIndex = endIndex;

		//update end index for last material
		(*ppMaterials)[curSubset].materials[addedMaterials-1].endIndex = addedIndices;//endIndex;

		//finally create vertex and index buffers for current subset
		//(*ppVertexBuffers)[curSubset].Init(v, sizeof(Bump_Vertex), addedVertices);
		data.SetData(posArray);
		if (!((*ppVerticesBuffers)[curSubset].Create(&posBufferParams, &data, addedVertices, sizeof(float3))))
		{
			assert(0 && "Error creating vertex buffer!");
		}

		data.SetData(texCoordArray);
		if (!((*ppTexCoordBuffers)[curSubset].Create(&texCoordBufferParams, &data, addedVertices, sizeof(float2))))
		{
			assert(0 && "Error creating vertex buffer!");
		}

		data.SetData(normalDataArray);
		if (!((*ppNormalBuffers)[curSubset].Create(&normalBufferParams, &data, addedVertices, sizeof(NormalData))))
		{
			assert(0 && "Error creating vertex buffer!");
		}

		(*ppIndexBuffers)[curSubset++].Init(indices, sizeof(DWORD), addedIndices);
	}
	#pragma endregion */

	//m_pScene = m_Importer.ReadFileFromMemory(pObjStream, bufferLength, 
	//											aiProcess_CalcTangentSpace	|
//												aiProcess_Triangulate		|
	//											aiProcess_ConvertToLeftHanded);
	m_pScene = m_Importer.ReadFile(pObjStream, aiProcess_CalcTangentSpace	|
												aiProcess_Triangulate		|
												aiProcess_ConvertToLeftHanded);
	if (!m_pScene)
	{
		const ACHAR * pError = m_Importer.GetErrorString();
		assert(0 && m_Importer.GetErrorString());
		return false;
	}

	//set  default values to 0
	AREAL xRad, yRad, zRad;
	xRad = yRad = zRad = 0;

	//initialize subsets
	pExtra->m_uNumMeshes = m_pScene->mNumMeshes;
	//pExtra->m_pMeshes = new IndexedMesh[pExtra->m_uNumMeshes];
	pExtra->m_pMeshes.reserve(pExtra->m_uNumMeshes);

	//Initialize all materials
	std::vector<Material*>		Materials;
	Materials.reserve(m_pScene->mNumMaterials);
 	for (int i = 0; i < m_pScene->mNumMaterials; i++)
	{
		Material*	pMat;
		aiString name;

		//initialize new material
		//pMat.reset(new Material);
		pMat = new Material();

		//load ambient texture
		m_pScene->mMaterials[i]->GetTexture(aiTextureType_AMBIENT, 0, &name);
		wchar_t* ambient = new wchar_t[name.length];
		char * ambstr = const_cast<char*>(name.C_Str());
		UnsupportedtoJPG(ambstr);
		AnsiToWideCch(ambient, /*name.C_Str() */ambstr, name.length+1);
		if (name.length > 0)
		{
			//pMat->m_pAmbientTexture->CreateFromFile(ambient);
			pMat->SetDiffuseTexture(ambient);
		}
		
		//load diffuse texture
		m_pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &name);
		wchar_t* diffuse = new wchar_t[name.length];
		char * difstr = const_cast<char*>(name.C_Str());
		UnsupportedtoJPG(difstr);
		AnsiToWideCch(diffuse, /*name.C_Str() */difstr, name.length+1);
		if (name.length > 0)
		{
			//pMat->m_pDiffuseTexture->CreateFromFile(diffuse);
			pMat->SetDiffuseTexture(diffuse);
		}

		//load bump map
		//m_pScene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &name);
		//wchar_t* normals = new wchar_t[name.length];
		//char * norstr = const_cast<char*>(name.C_Str());
		//UnsupportedtoJPG(norstr);
		//AnsiToWideCch(normals, /*name.C_Str() */norstr, name.length+1);
		//if (name.length > 0)
		//{
			//pMat->m_pBumpMap->CreateFromFile(normals);
		//	pMat->SetNormalTexture(normals);
		//}
		//load normal map
		char* ending = "_norm.png";
		const char* beg = name.C_Str();
		const char* pSep = strchr(beg, '_');
		if (pSep)
		{
			strcpy(const_cast<char*>(pSep), ending);
			wchar_t* normal = new wchar_t[name.length];
			AnsiToWideCch(normal, beg, name.length+1);
			//pMat->m_pHeightMap->CreateFromFile(heights);
			pMat->SetNormalTexture(normal);
		}

		//load height map
		ending = "_bump.png";
		beg = name.C_Str();
		pSep = strchr(beg, '_');
		if (pSep)
		{
			strcpy(const_cast<char*>(pSep), ending);
			wchar_t* heights = new wchar_t[name.length];
			AnsiToWideCch(heights, beg, name.length+1);
			//pMat->m_pHeightMap->CreateFromFile(heights);
			pMat->SetHeightTexture(heights);
		}

		//load specular map
		ending = "_spec.png";
		beg = name.C_Str();
		pSep = strchr(beg, '_');
		if (pSep)
		{
			strcpy(const_cast<char*>(pSep), ending);
			wchar_t* specs = new wchar_t[name.length];
			AnsiToWideCch(specs, beg, name.length+1);
			//pMat->m_pHeightMap->CreateFromFile(heights);
			pMat->SetSpecularTexture(specs);
		}

		//push material to the list
		Materials.push_back(pMat);
	}

	//extern void LoadMesh();
	//iterate through all meshes
	for (int i = 0; i < pExtra->m_uNumMeshes; i++)
	{
		pExtra->m_pMeshes.push_back(make_shared<IndexedMesh>(IndexedMesh()));

		//Create VertexBuffer
		int numVertices = m_pScene->mMeshes[i]->mNumVertices;

		if (!m_pScene->mMeshes[i]->HasBones())
		{
#pragma region Not Animated Mesh
			//Bump_Vertex* v;
			//v = new Bump_Vertex[numVertices];

			BufferParams posBufferParams, texCoordBufferParams, normalBufferParams;
			posBufferParams.FillVertexBufferParams(sizeof(float3), numVertices, true, false, false, false);
			texCoordBufferParams.FillVertexBufferParams(sizeof(float2), numVertices, true, false, false, false);
			normalBufferParams.FillVertexBufferParams(sizeof(NormalData), numVertices, true, false, false, false); 

			SubresourceData data;

			float3* pos = new float3[numVertices];
			float2* texCoord = new float2[numVertices];
			NormalData* normalData = new NormalData[numVertices];

			for (int j = 0; j < numVertices; j++)
			{
				//v[j].pos		= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mVertices[j]);
				pos[j] = aiVector3D_to_float3(m_pScene->mMeshes[i]->mVertices[j]);

				//float temp = v[j].pos.y;
				//v[j].pos.y = v[j].pos.z;
				//v[j].pos.z = temp;

				//v[j].normal		= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mNormals[j]);
				normalData[j].normal = aiVector3D_to_float3(m_pScene->mMeshes[i]->mNormals[j]);

				if (m_pScene->mMeshes[i]->mTextureCoords[0])
				{
					//v[j].texCoord	= aiVector3D_to_Vec2(m_pScene->mMeshes[i]->mTextureCoords[0][j]);
					texCoord[j] = aiVector3D_to_float2(m_pScene->mMeshes[i]->mTextureCoords[0][j]);
				}
				if (m_pScene->mMeshes[i]->mTangents)
				{
					//v[j].tangent	= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mTangents[j]);
					normalData[j].tangent = aiVector3D_to_float3(m_pScene->mMeshes[i]->mTangents[j]);
				}
				if (m_pScene->mMeshes[i]->mBitangents)	
				{
					//v[j].binormal	= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mBitangents[j]);
					normalData[j].binormal = aiVector3D_to_float3(m_pScene->mMeshes[i]->mBitangents[j]);
				}

				//Mat4x4 rotX;
				//rotX.BuildRotationX(3.14f / 2.0f);
				//Mat4x4 rotY;
				//rotY.BuildRotationY(3.14f / 2.0f);

				if (abs(pos[j].x) > xRad) xRad = abs(pos[j].x);
				if (abs(pos[j].y) > yRad) yRad = abs(pos[j].y);
				if (abs(pos[j].z) > zRad) zRad = abs(pos[j].z);
			}

			data.SetData(pos);
			pExtra->m_pMeshes[i]->m_pVertices = new VertexBuffer();
			pExtra->m_pMeshes[i]->m_pVertices->Create(&posBufferParams, &data, numVertices, sizeof(float3));

			data.SetData(texCoord);
			pExtra->m_pMeshes[i]->m_pTexCoords = new VertexBuffer();
			pExtra->m_pMeshes[i]->m_pTexCoords->Create(&texCoordBufferParams, &data, numVertices, sizeof(float2));

			data.SetData(normalData);
			pExtra->m_pMeshes[i]->m_pNormals = new VertexBuffer();
			pExtra->m_pMeshes[i]->m_pNormals->Create(&normalBufferParams, &data, numVertices, sizeof(NormalData));
			//pExtra->m_pSubsets[i].m_pVertexBuffer = new VertexBuffer(v, sizeof(Bump_Vertex), 
			//	numVertices);
#pragma endregion
		}
		else
		{ /*
#pragma region Animated Mesh
			//initialize vertices
			Skeletal_Vertex* v;
			v = new Skeletal_Vertex[numVertices];

			const unsigned __int32 numBones = m_pScene->mMeshes[i]->mNumBones;
			(*ppSubsets)[i].m_numBones = numBones;

			for (int vert = 0; vert < numVertices; vert++)
			{
				v[vert].pos				= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mVertices[vert]);
				v[vert].bonesIndices[0]	= numBones; 
				v[vert].bonesIndices[1]	= numBones; 
				v[vert].bonesIndices[2]	= numBones; 
				v[vert].bonesIndices[3]	= numBones;
				v[vert].bonesWeights	= Vec4(0.0f, 0.0f, 0.0f, 0.0f);
				if (m_pScene->mMeshes[i]->mTextureCoords[0])
					v[vert].texCoord		= aiVector3D_to_Vec2(m_pScene->mMeshes[i]->mTextureCoords[0][vert]);
				if (m_pScene->mMeshes[i]->mNormals)
					v[vert].normal			= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mNormals[vert]);
				if (m_pScene->mMeshes[i]->mTangents)
					v[vert].tangent			= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mTangents[vert]);
				if (m_pScene->mMeshes[i]->mBitangents)
					v[vert].binormal		= aiVector3D_to_Vec3(m_pScene->mMeshes[i]->mBitangents[vert]);

				if (abs(v[vert].pos.x) > xRad) xRad = abs(v[vert].pos.x);
				if (abs(v[vert].pos.y) > yRad) yRad = abs(v[vert].pos.y);
				if (abs(v[vert].pos.z) > zRad) zRad = abs(v[vert].pos.z);
			}

			//handle bones data
			(*ppSubsets)[i].m_pBones = new Bone[numBones];
			for (int bone = 0; bone < numBones; bone++)
			{
				aiBone const * const pBone = m_pScene->mMeshes[i]->mBones[bone];

				//set trasnform from mesh space to bone space
				(*ppSubsets)[i].m_pBones[bone].SetObjectSpaceTransform(aiMatrix4x4_to_Mat4x4(pBone->mOffsetMatrix), true);

				int anim = 0;
				//get animation data
				int channels = m_pScene->mAnimations[anim]->mNumChannels;
				(*ppSubsets)[i].m_pBones[bone].m_frames = m_pScene->mAnimations[anim]->mChannels[bone]->mNumPositionKeys;
				//for (int n= 0; n < channels; n++)
				//{
					for (int k = 0; k < m_pScene->mAnimations[anim]->mChannels[bone]->mNumPositionKeys; k++)
					{
						//get key position
						Vec3 key = aiVector3D_to_Vec3(m_pScene->mAnimations[anim]->mChannels[bone]->mPositionKeys[k].mValue);
						Mat4x4 trans;
						trans.BuildTranslation(key);

						//get rotation
						Quaternion q = aiQuaternion_to_Quaternion(m_pScene->mAnimations[anim]->mChannels[bone]->mRotationKeys[k].mValue);
						Mat4x4 rot = q.BuildMatrix();
							
						Mat4x4 mat(rot * trans);

						//get time
						double timePos = m_pScene->mAnimations[anim]->mChannels[bone]->mPositionKeys[k].mTime;

						//finally add new frame
						(*ppSubsets)[i].m_pBones[bone].m_sequence.push_back(std::make_pair(timePos, mat));
					}
					//}

				//now we are gonna generate none ids and weights
				for (int k = 0; k < pBone->mNumWeights; k++)
				{
					unsigned __int32 vertexId = pBone->mWeights[k].mVertexId;
					float			 weight   = pBone->mWeights[k].mWeight;

					if (bone == v[vertexId].bonesIndices[0] || bone == v[vertexId].bonesIndices[1] ||
						bone == v[vertexId].bonesIndices[2] || bone == v[vertexId].bonesIndices[3])
					{
						assert(0 && "Bone affects the vertex twice?!");
					}

					//this is the first bone associated with vertex(vertexId)
					if (v[vertexId].bonesIndices[0] == numBones)
					{
						v[vertexId].bonesIndices[0] = bone;
						v[vertexId].bonesWeights.x = weight;
					}
					//second
					else if (v[vertexId].bonesIndices[1] == numBones)
					{
						v[vertexId].bonesIndices[1] = bone;
						v[vertexId].bonesWeights.y = weight;
					}
					//third
					else if (v[vertexId].bonesIndices[2] == numBones)
					{
						v[vertexId].bonesIndices[2] = bone;
						v[vertexId].bonesWeights.z = weight;
					}
					//fourth
					else if (v[vertexId].bonesIndices[3] == numBones)
					{
						v[vertexId].bonesIndices[3] = bone;
						v[vertexId].bonesWeights.w = weight;
					}
					else
					{
						unsigned __int32 ind0 = v[vertexId].bonesIndices[0];
						unsigned __int32 ind1 = v[vertexId].bonesIndices[1];
						unsigned __int32 ind2 = v[vertexId].bonesIndices[2];
						unsigned __int32 ind3 = v[vertexId].bonesIndices[3];

						float weight0 = v[vertexId].bonesWeights.x;
						float weight1 = v[vertexId].bonesWeights.y;
						float weight2 = v[vertexId].bonesWeights.z;
						float weight3 = v[vertexId].bonesWeights.w;

						assert(0 && "Maximum four bones can affect the vertex!");
					}
				}
			}

			(*ppSubsets)[i].m_pVertexBuffer = new VertexBuffer(v, sizeof(Skeletal_Vertex), 
				numVertices);
#pragma endregion */
		}

		//Create IndexBuffer
		int numFaces = m_pScene->mMeshes[i]->mNumFaces;
		DWORD* indices;
		indices = new DWORD[numFaces * 3];
		for (int j = 0; j < numFaces; j++)
		{
			indices[j*3] = m_pScene->mMeshes[i]->mFaces[j].mIndices[0];
			indices[(j*3)+1] = m_pScene->mMeshes[i]->mFaces[j].mIndices[1];
			indices[(j*3)+2] = m_pScene->mMeshes[i]->mFaces[j].mIndices[2];
		}

		//pExtra->m_pSubsets[i].m_pIndexBuffer = new IndexBuffer(indices, sizeof(DWORD),
		//	numFaces * 3);
		BufferParams indexBufferParams;
		indexBufferParams.FillIndexBufferParams(sizeof(DWORD), numFaces*3, true, false, false, false);
		SubresourceData data;
		data.SetData(indices);

		pExtra->m_pMeshes[i]->m_pIndexBuffer = new IndexBuffer();
		pExtra->m_pMeshes[i]->m_pIndexBuffer->Create(&indexBufferParams, &data, numFaces*3, sizeof(DWORD));


		//load material

		pExtra->m_pMeshes[i]->m_pMaterial = Materials[m_pScene->mMeshes[i]->mMaterialIndex];
	}

	return true;
}
