#pragma once
#include "head.hpp"

struct SPos;
class CEntity;

struct SPos
{
	float x, y, z;

	SPos(const float& x, const float& y, const float& z);
};

class CEntity
{
private:
	struct SPos pos;

public:
	CEntity();
	CEntity(const float& x, const float& y, const float& z);

public:
	void SetPos(const float& x, const float& y, const float& z);
	const SPos& GetPos() const;
};