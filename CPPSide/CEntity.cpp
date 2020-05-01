#include "CEntity.hpp"
SPos::SPos(const float& _x, const float& _y, const float& _z) : 
	x(_x),
	y(_y),
	z(_z)
{
	
}

CEntity::CEntity() :
	pos(0.0f, 0.0f, 0.0f)
{

}

CEntity::CEntity(const float& x, const float& y, const float& z) : 
	pos(x, y ,z)
{
	
}

void CEntity::SetPos(const float& x, const float& y, const float& z)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
}

const SPos& CEntity::GetPos() const
{
	return this->pos;
}