#include "QuadTreeObject.h"

QuadTreeObject::QuadTreeObject(TerrainChunk * chunk)
{
	m_chunk = chunk;
	m_entity = nullptr;
	m_light = nullptr;
}

QuadTreeObject::QuadTreeObject(Entity * entity)
{
	m_chunk = nullptr;
	m_entity = entity;
	m_light = nullptr;
}

QuadTreeObject::QuadTreeObject(Light * light)
{
	m_chunk = nullptr;
	m_entity = nullptr;
	m_light = light;
}

TerrainChunk * QuadTreeObject::getChunk() const
{
	return m_chunk;
}

Entity * QuadTreeObject::getEntity() const
{
	return m_entity;
}

Light * QuadTreeObject::getLight() const
{
	return m_light;
}
