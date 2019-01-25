#ifndef _MESH_MEMORY_COLLECTOR_H
#define _MESH_MEMORY_COLLECTOR_H
#include <vector>
#include "Meshes/Mesh.h"
#include "Meshes/InstancedMesh.h"

class Application;
// Cleans up the memory of the mesh at the end of the application
class MeshMemoryCollector
{
public:
	static void registerMesh(Mesh* mesh);
	static void registerMesh(InstancedMesh* mesh);
private:
	friend class Application;
	MeshMemoryCollector();
	~MeshMemoryCollector();
	static std::vector<Mesh*> m_meshes;
	static std::vector<InstancedMesh*> m_instancedMeshes;
};

#endif

