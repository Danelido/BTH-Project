#ifndef _QUAD_TREE_OBJECTS_H
#define _QUAD_TREE_OBJECTS_H
#include "Terrain/TerrainChunk.h"
#include "Entity/Entity.h"


class QuadTreeObject
{
public:

	QuadTreeObject(TerrainChunk* chunk);
	QuadTreeObject(Entity* entity);

	TerrainChunk* getChunk() const; /* MAY BE NULL IF NOT INITIALIZED*/
	Entity* getEntity() const; /* MAY BE NULL IF NOT INITIALIZED*/

private:
	TerrainChunk* m_chunk;
	Entity* m_entity;


};


#endif
