#include "MeshMemoryCollector.h"
std::vector<Mesh*> MeshMemoryCollector::m_meshes;
std::vector<InstancedMesh*> MeshMemoryCollector::m_instancedMeshes;

MeshMemoryCollector::MeshMemoryCollector()
{
	m_meshes.reserve(1000);
}


MeshMemoryCollector::~MeshMemoryCollector()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		if(m_meshes[i])
			delete m_meshes[i];
	}

	for (size_t i = 0; i < m_instancedMeshes.size(); i++)
	{
		if (m_instancedMeshes[i])
			delete m_instancedMeshes[i];
	}
}

void MeshMemoryCollector::registerMesh(Mesh * mesh)
{
	m_meshes.emplace_back(mesh);
}

void MeshMemoryCollector::registerMesh(InstancedMesh * mesh)
{
	m_instancedMeshes.emplace_back(mesh);
}
