#include "VSMDemo_pch.h"

#include "PlaneEntity.h"


#include "../../../Renderer/Meshes/PlaneMesh.h"
#include "Data\JSONLoader.h"
#include "Application\Engine.h"
#include "../../Math/Source/Mat4x4.h"

#include <Allocator.h>

using namespace Anubis;

////////////////////////////////////////////////////////////
//PlaneEntitiy Resource
////////////////////////////////////////////////////////////

AVOID PlaneEntityResource::VLoad(const ASTRING & filename)
{
	EntityResource::VLoad(filename);

	//load size
	m_r32Width = JSONLoader::GetAREAL32("width");
	m_r32Height = JSONLoader::GetAREAL32("height");
}

EntityResource & PlaneEntityResource::VCopy()
{
	return static_cast<EntityResource &>(*this);
}

//create entity
EntityPtr PlaneEntityResource::VCreateEntity(Game * pGame)
{
	return pGame->VAddEntity(make_shared<Entity>(PlaneEntity(pGame->GetNewEntityId())));
}

EntityRepresentation* PlaneEntityResource::VCreateRepresentation(Scene * pScene, EntityPtr pEntity)
{
	Allocator allocator;
	APVOID pMemory = allocator.allocateAligned(sizeof(PlaneEntityRepresentation), 16);

	PlaneEntityRepresentation * pPlanePointer = new (pMemory) PlaneEntityRepresentation(pEntity->GetId(), L"diffusemap.jpg", L"normalmap.jpg",
		L"specularmap.jpg", L"heightmap.jpg", float2(m_r32Width, m_r32Height));
	//PlaneEntityRepresentation * pPlanePointer = new (pMemory) PlaneEntityRepresentation(pEntity->GetId(), L"chest_diff.jpg", L"chest_norm.jpg",
	//	L"chest_spec.jpg", L"chest_bump.jpg", float2(m_r32Width, m_r32Height));
	pPlanePointer->SetSize(m_r32Width, m_r32Height, 1.0f);
	pEntity->SetRepresentation(pPlanePointer);

	Mat4x4 trans;
	trans.CreateTranslation(m_pos);
	pPlanePointer->SetCurrentTransform(trans, g_pEngine->GameTimeInSeconds());
	pScene->AddRepresentation(pPlanePointer);

	return pPlanePointer;
}

AVOID PlaneEntityResource::VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity)
{
	pPhysics->VAddPlane(pEntity, m_pos, Vector(m_r32Width, m_r32Height, 0.0f, 0.0f), 0, ASTRING("Normal"), true);
}

////////////////////////////////////////////////////////////
//PlaneEntity Representation
////////////////////////////////////////////////////////////
PlaneEntityRepresentation::PlaneEntityRepresentation(EntityRepresentationId id, AWSTRING diffuseMaterialFile, AWSTRING normalMaterialFile,
							AWSTRING specularMaterialFile, AWSTRING heightMaterialFile, const float2 & f2Size)
							: EntityRepresentation(id)
{
	PlaneMesh* pPlaneMesh = new PlaneMesh("", f2Size.x, f2Size.y);
	pPlaneMesh->VSetMaterial(diffuseMaterialFile, normalMaterialFile, specularMaterialFile, heightMaterialFile);
	shared_ptr<PlaneMesh> pPlaneMeshPtr = make_shared<PlaneMesh>(*pPlaneMesh);
	m_meshes.push_back(pPlaneMeshPtr);
}