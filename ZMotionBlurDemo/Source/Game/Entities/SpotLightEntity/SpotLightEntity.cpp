#include "VSMDemo_pch.h"
#include "SpotLightEntity.h"

#include "SpotLightEntityRepresentation.h"

AVOID SpotLightEntity::SetDirection(const Vec & dir)
{
	SpotLightEntityRepresentation* pRepr = static_cast<SpotLightEntityRepresentation*>(m_pRepresentation);
	pRepr->SetDirection(dir);
}
