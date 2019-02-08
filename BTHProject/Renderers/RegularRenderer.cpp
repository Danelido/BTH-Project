#include "RegularRenderer.h"

RegularRenderer::RegularRenderer(const glm::mat4& projectionMatrix)
{
	m_meshShader = new MeshShader();
	m_meshShader->use();
	m_meshShader->setProjectionMatrix(projectionMatrix);
	m_meshShader->unuse();
}


RegularRenderer::~RegularRenderer()
{
	delete m_meshShader;
}

void RegularRenderer::submit(Entity * entity)
{
	
	m_meshIterator = m_meshes.find(entity->getMesh());

	if (m_meshIterator != m_meshes.end())
	{
		m_meshIterator->second.emplace_back(entity);
	}
	else
	{
		std::vector<Entity*> meshVec;
		meshVec.reserve(MESH_VECTOR_RESERVE_SIZE);
		meshVec.emplace_back(entity);
		m_meshes.emplace(entity->getMesh(), meshVec);
	}
	
}

void RegularRenderer::render(const FPSCamera * activeCamera, const FPSCamera* mainCamera)
{
	if (m_meshes.size() == 0)
		return;

	m_meshShader->use();
	m_meshShader->setViewMatrix(activeCamera->getViewMatrix());
	for (auto &meshMap : m_meshes)
	{
		bindMesh(meshMap.first, mainCamera);

		for (auto entity : meshMap.second)
		{
			updateModelMatrix(entity);
			glDrawElements(GL_TRIANGLES, entity->getMesh()->getIndicesSize(), GL_UNSIGNED_INT, NULL);
		}

		unbindMesh(meshMap.first);
	}

	m_meshShader->unuse();
	m_meshes.clear();
}

void RegularRenderer::bindMesh(Mesh * mesh, const FPSCamera* camera)
{
	glBindVertexArray(mesh->getVao());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	this->m_meshShader->setCameraPosition(camera->getPosition());
	for (int i = 0; i < mesh->getTexIDs().size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->getTexIDs()[i]);
	}
	
}

void RegularRenderer::updateModelMatrix(Entity * entity)
{
	m_meshShader->setModelMatrix(entity->getModelMatrix());
}

void RegularRenderer::unbindMesh(Mesh * mesh)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glBindVertexArray(NULL);
}
