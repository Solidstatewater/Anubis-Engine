#include "TestGame_pch.h"
#include "BoxEntityRepresentation.h"

#include "../../Renderer/Meshes/BoxMesh.h"

BoxEntityRepresentation::BoxEntityRepresentation(EntityRepresentationId id, AWSTRING diffuseMaterialFile, AWSTRING normalMaterialFile,
							AWSTRING specularMaterialFile, AWSTRING heightMaterialFile, const float3 & f3Size)
							: DynamicRepresentation(id, f3Size)
{
	BoxMesh* pBoxMesh = new BoxMesh("", m_f3Size.x, m_f3Size.y, m_f3Size.z);
	pBoxMesh->VSetMaterial(diffuseMaterialFile, normalMaterialFile, specularMaterialFile, heightMaterialFile);
	shared_ptr<BoxMesh> pBoxMeshPtr = make_shared<BoxMesh>(*pBoxMesh);
	m_meshes.push_back(pBoxMeshPtr);
}