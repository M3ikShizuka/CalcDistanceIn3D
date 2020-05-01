#pragma once
#include "head.h"

struct s_pos;
struct s_entity;

struct s_pos
{
	float x, y, z;
};

struct s_entity
{
	struct s_pos pos;
};

#ifndef DATA_LIST
const struct s_entity* s_entity(const float x, const float y, const float z);

const struct s_entity* s_entity(const float x, const float y, const float z)
{
	struct s_entity* p_entity = malloc(sizeof(struct s_entity));
	p_entity->pos.x = x;
	p_entity->pos.y = y;
	p_entity->pos.z = z;

	return p_entity;
}
#endif