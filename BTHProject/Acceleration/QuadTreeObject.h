#ifndef _QUAD_TREE_OBJECTS_H
#define _QUAD_TREE_OBJECTS_H
#include "Terrain/TerrainChunk.h"
#include "Entity/Entity.h"
#include "Light/Light.h"

class QuadTreeObject
{
public:

	QuadTreeObject(TerrainChunk* chunk);
	QuadTreeObject(Entity* entity);
	QuadTreeObject(Light* light);

	TerrainChunk* getChunk() const; /* MAY BE NULL IF NOT INITIALIZED*/
	Entity* getEntity() const; /* MAY BE NULL IF NOT INITIALIZED*/
	Light* getLight() const; /* MAY BE NULL IF NOT INITIALIZED*/

private:
	TerrainChunk* m_chunk;
	Entity* m_entity;
	Light* m_light;


};


#endif
