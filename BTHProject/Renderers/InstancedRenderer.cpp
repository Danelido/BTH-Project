#include "InstancedRenderer.h"


InstancedRenderer::InstancedRenderer(const glm::mat4& projectionMatrix)
{
	m_instancedShader = new InstancedMeshShader();
	m_instancedShader->use();
	m_instancedShader->setProjectionMatrix(projectionMatrix);
	m_instancedShader->unuse();
}

InstancedRenderer::~InstancedRenderer()
{
	delete m_instancedShader;
}

void InstancedRenderer::submit(Entity * entity)
{
	m_instancedMeshIterator = m_instancedMeshes.find(entity->getInstancedMesh());

	if (m_instancedMeshIterator != m_instancedMeshes.end())
	{
		m_instancedMeshIterator->second.emplace_back(entity);
	}
	else
	{
		std::vector<Entity*> instancedMeshVec;
		instancedMeshVec.reserve(MESH_VECTOR_RESERVE_SIZE);
		instancedMeshVec.emplace_back(entity);
		m_instancedMeshes.emplace(entity->getInstancedMesh(), instancedMeshVec);
	}
}

void InstancedRenderer::render(const FPSCamera* activeCamera, const FPSCamera* mainCamera)
{
	if (m_instancedMeshes.size() == 0) return;

	m_instancedShader->use();
	m_instancedShader->setViewMatrix(activeCamera->getViewMatrix());

	for (auto &instancedMeshMap : m_instancedMeshes)
	{
		if (instancedMeshMap.second.size() == 0)
			continue;

		begin(instancedMeshMap.first, instancedMeshMap.second.size());

		for (auto entity : instancedMeshMap.second)
		{
			prepare(entity);
		}
		
		end();
		
		flush(instancedMeshMap.first, instancedMeshMap.second.size(), mainCamera);
		
		m_modelMatrices.clear();
	}
	m_instancedShader->unuse();
	m_instancedMeshes.clear();
}

void InstancedRenderer::begin(InstancedMesh* mesh, size_t quantity)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh->getModelBufferID());
	glBufferData(GL_ARRAY_BUFFER, 16 * quantity * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	m_modelMatrices.reserve(quantity);
}

void InstancedRenderer::prepare(Entity * entity)
{
	m_modelMatrices.emplace_back(entity->getModelMatrix());
}

void InstancedRenderer::end()
{
	glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * m_modelMatrices.size() * sizeof(float), m_modelMatrices.data());
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void InstancedRenderer::flush(InstancedMesh* mesh, size_t quantity, const FPSCamera* camera)
{
	bindMesh(mesh, camera);
	glDrawElementsInstanced(GL_TRIANGLES, mesh->getIndicesSize(), GL_UNSIGNED_INT, 0, (GLsizei)quantity);
	unbindMesh(mesh);
}

void InstancedRenderer::bindMesh(InstancedMesh * mesh, const FPSCamera* camera)
{
	glBindVertexArray(mesh->getVao());

	for(GLuint i = 0; i <= 6; i++)
		glEnableVertexAttribArray(i);
	
	m_instancedShader->setCameraPosition(camera->getPosition());
	
	for (int i = 0; i < mesh->getTexIDs().size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->getTexIDs()[i]);
	}
	
}

void InstancedRenderer::unbindMesh(InstancedMesh * mesh)
{
	for (GLuint i = 0; i <= 6; i++)
		glDisableVertexAttribArray(i);
	
	glBindTexture(GL_TEXTURE_2D, NULL);
	glBindVertexArray(NULL);
}
