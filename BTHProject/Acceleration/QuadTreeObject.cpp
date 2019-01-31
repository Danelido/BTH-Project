#include "QuadTreeObject.h"

QuadTreeObject::QuadTreeObject(TerrainChunk * chunk)
{
	m_chunk = chunk;
	m_entity = nullptr;
}

QuadTreeObject::QuadTreeObject(Entity * entity)
{
	m_chunk = nullptr;
	m_entity = entity;
}

TerrainChunk * QuadTreeObject::getChunk() const
{
	return m_chunk;
}

Entity * QuadTreeObject::getEntity() const
{
	return m_entity;
}
