#include "FileStream_pch.h"
#include "MeshLoader.h"
/*
ABOOL MeshLoader::LoadObj(const AWSTRING fileName, float3** pVertices,
			float2** pTexCoords, float3** pNormals, MaterialsKeeper** pMaterials,
			AUINT32 & numSubsets)
{
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
	*ppVertexBuffers = new VertexBuffer[numSubsets];
	*ppIndexBuffers = new IndexBuffer[numSubsets];
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
		Bump_Vertex* v = new Bump_Vertex[uniqueIndices];
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

				//v[addedVertices++] = Bump_Vertex(positionList[vertexData.posIndex].x,
				//								 positionList[vertexData.posIndex].y,
				//								 positionList[vertexData.posIndex].z,
				//								 texCoordList[vertexData.texCoordIndex].x,
				//								 texCoordList[vertexData.texCoordIndex].y,
				//								 normalList[vertexData.normalIndex].x,
				//								 normalList[vertexData.normalIndex].y,
				//								 normalList[vertexData.normalIndex].z);
				XMFLOAT3 pos(posX, posY, posZ);
				XMFLOAT2 tex(texU, texV);
				XMFLOAT3 normal(normalX, normalY, normalZ);

				Bump_Vertex vertex(posX, posY, posZ, texU, texV, normalX, normalY, normalZ);

				//v[addedVertices++] = Bump_Vertex(posX,
				//								 posY,
				//								 posZ,
				//								 texU,
				//								 texV,
				//								 normalX,
				//								 normalY,
				//								 normalZ); 
				v[addedVertices++] = vertex;
			}
		}

		startIndex = endIndex;

		//update end index for last material
		(*ppMaterials)[curSubset].materials[addedMaterials-1].endIndex = addedIndices;//endIndex;

		//finally create vertex and index buffers for current subset
		(*ppVertexBuffers)[curSubset].Init(v, sizeof(Bump_Vertex), addedVertices);
		(*ppIndexBuffers)[curSubset++].Init(indices, sizeof(DWORD), addedIndices);
	}
	#pragma endregion
} */