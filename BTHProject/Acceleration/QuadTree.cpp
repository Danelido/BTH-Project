#include "QuadTree.h"
#include "App/AppSettings.h"


QuadTree::QuadTree(AABB boundary, FPSCamera* camera)
{
	this->m_boundary = boundary;
	this->m_camera = camera;
	m_northWest = nullptr;
	m_northEast = nullptr;
	m_southWest = nullptr;
	m_southEast = nullptr;
	m_treeObjects.reserve(QT_NODE_CAPACITY);
	if(AppSettings::DEBUG_LAYER())
		createGLDrawable();
}

QuadTree::~QuadTree()
{

	for (size_t i = 0; i < m_treeObjects.size(); i++)
		delete m_treeObjects[i];

	if(m_northWest)
		delete m_northWest;
	
	if(m_northEast)
		delete m_northEast;

	if(m_southWest)
		delete m_southWest;

	if(m_southEast)
		delete m_southEast;

	if (AppSettings::DEBUG_LAYER()) {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

}

bool QuadTree::insert(TerrainChunk * chunk)
{
	if (!m_boundary.containsTerrainChunk(chunk))
		return false;

	if (m_treeObjects.size() < QT_NODE_CAPACITY && m_northWest == nullptr)
	{
		m_treeObjects.emplace_back(new QuadTreeObject(chunk));
		return true;
	}

	if (m_northWest == nullptr)
	{
		subDivide();
	}

	if (m_northWest->insert(chunk)) return true;
	if (m_northEast->insert(chunk)) return true;
	if (m_southWest->insert(chunk)) return true;
	if (m_southEast->insert(chunk)) return true;

	return false;
}

bool QuadTree::insert(Light * light)
{
	if (!m_boundary.containsLight(light))
		return false;

	if (m_treeObjects.size() < QT_NODE_CAPACITY && m_northWest == nullptr)
	{
		m_treeObjects.emplace_back(new QuadTreeObject(light));
		return true;
	}

	if (m_northWest == nullptr)
	{
		subDivide();
	}

	if (m_northWest->insert(light)) return true;
	if (m_northEast->insert(light)) return true;
	if (m_southWest->insert(light)) return true;
	if (m_southEast->insert(light)) return true;

	return false;
}

bool QuadTree::insert(Entity * entity)
{
	if (!m_boundary.containsEntity(entity))
		return false;



	if (m_treeObjects.size() < QT_NODE_CAPACITY && m_northWest == nullptr)
	{
		m_treeObjects.emplace_back(new QuadTreeObject(entity));
		return true;
	}

	if (m_northWest == nullptr)
	{
		subDivide();
	}

	if (m_northWest->insert(entity)) return true;
	if (m_northEast->insert(entity)) return true;
	if (m_southWest->insert(entity)) return true;
	if (m_southEast->insert(entity)) return true;

	return false;
}

std::vector<QuadTreeObject*> QuadTree::query()
{
	std::vector<QuadTreeObject*> objects;
	objects.reserve(1500);
	
	if (!m_camera->insideFrustum(m_boundary))
		return objects;

	for (size_t i = 0; i < m_treeObjects.size(); i++)
	{
		// Is the objects in this section inside the frustum?
		if (m_treeObjects[i]->getEntity() != nullptr)
		{
			if (m_camera->insideFrustum(m_treeObjects[i]->getEntity()->getBoundary()))
				objects.emplace_back(m_treeObjects[i]);
		}

		else if (m_treeObjects[i]->getChunk() != nullptr)
		{
			if (m_camera->insideFrustum(m_treeObjects[i]->getChunk()->getBoundary()))
				objects.emplace_back(m_treeObjects[i]);
		}

		else if (m_treeObjects[i]->getLight() != nullptr)
		{
			if (m_camera->insideFrustum(m_treeObjects[i]->getLight()->getPosition(), m_treeObjects[i]->getLight()->getRadius()))
				objects.emplace_back(m_treeObjects[i]);
		}
	}

	if (m_northWest == nullptr)
		return objects;

	std::vector<QuadTreeObject*> nwObjects = m_northWest->query();
	for (size_t i = 0; i < nwObjects.size(); i++)
		objects.emplace_back(nwObjects[i]);

	std::vector<QuadTreeObject*> neObjects = m_northEast->query();
	for (size_t i = 0; i < neObjects.size(); i++)
		objects.emplace_back(neObjects[i]);

	std::vector<QuadTreeObject*> swObjects = m_southWest->query();
	for (size_t i = 0; i < swObjects.size(); i++)
		objects.emplace_back(swObjects[i]);

	std::vector<QuadTreeObject*> seObjects = m_southEast->query();
	for (size_t i = 0; i < seObjects.size(); i++)
		objects.emplace_back(seObjects[i]);

	return objects;
}

void QuadTree::subDivide()
{
	// North West
	XYZ nwP(m_boundary.center.x - (m_boundary.halfDimensions.x * 0.5f), m_boundary.center.y, m_boundary.center.z + (m_boundary.halfDimensions.x * 0.5f));
	AABB nwB(nwP, m_boundary.halfDimensions.x * 0.5f, m_boundary.halfDimensions.y,  m_boundary.halfDimensions.z * 0.5f);
	m_northWest = new QuadTree(nwB, m_camera);

	// North East
	XYZ neP(m_boundary.center.x + (m_boundary.halfDimensions.x * 0.5f), m_boundary.center.y, m_boundary.center.z + (m_boundary.halfDimensions.z * 0.5f));
	AABB neB(neP, m_boundary.halfDimensions.x * 0.5f, m_boundary.halfDimensions.y, m_boundary.halfDimensions.z * 0.5f);
	m_northEast = new QuadTree(neB, m_camera);

	// South West
	XYZ swP(m_boundary.center.x - (m_boundary.halfDimensions.x * 0.5f), m_boundary.center.y, m_boundary.center.z - (m_boundary.halfDimensions.z * 0.5f));
	AABB swB(swP, m_boundary.halfDimensions.x * 0.5f, m_boundary.halfDimensions.y, m_boundary.halfDimensions.z * 0.5f);
	m_southWest = new QuadTree(swB, m_camera);

	// South East
	XYZ seP(m_boundary.center.x + (m_boundary.halfDimensions.x * 0.5f), m_boundary.center.y, m_boundary.center.z - (m_boundary.halfDimensions.z * 0.5f));
	AABB seB(seP, m_boundary.halfDimensions.x * 0.5f, m_boundary.halfDimensions.y, m_boundary.halfDimensions.z * 0.5f);
	m_southEast = new QuadTree(seB, m_camera);
}

void QuadTree::createGLDrawable()
{

	float data[8 * 3] = 
	{
		// 1
		m_boundary.center.x - m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z - m_boundary.halfDimensions.z,


		// 2
		m_boundary.center.x - m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z + m_boundary.halfDimensions.z,


		// 3
		m_boundary.center.x - m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z + m_boundary.halfDimensions.z,


		// 4
		m_boundary.center.x + m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z + m_boundary.halfDimensions.z,



		// 5
		m_boundary.center.x + m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z + m_boundary.halfDimensions.z,


		// 6
		m_boundary.center.x + m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z - m_boundary.halfDimensions.z,

		// 7
		m_boundary.center.x + m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z - m_boundary.halfDimensions.z,


		// 8
		m_boundary.center.x - m_boundary.halfDimensions.x,
		m_boundary.center.y,
		m_boundary.center.z - m_boundary.halfDimensions.z,

	};

	m_numVertices = 8;
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void QuadTree::draw(QuadTreeDebugShader* shader)
{
	if (AppSettings::DEBUG_LAYER())
	{
		if (m_camera->insideFrustum(m_boundary)) {
			shader->setColor(glm::vec3(1.0, 1.0f, 1.0f));
			glBindVertexArray(m_vao);
			glEnableVertexAttribArray(0);
			glDrawArrays(GL_LINES, 0, m_numVertices);
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);

			shader->setColor(glm::vec3(0.0, 0.5f, 0.5f));

			for (size_t i = 0; i < m_treeObjects.size(); i++)
			{
				if (m_treeObjects[i]->getEntity() != nullptr)
				{
					glBindVertexArray(m_treeObjects[i]->getEntity()->getDebugVAO());
					glEnableVertexAttribArray(0);
					glDrawArrays(GL_LINES, 0, 24);
					glDisableVertexAttribArray(0);
					glBindVertexArray(0);
				}
				else if (m_treeObjects[i]->getChunk() != nullptr)
				{
					glBindVertexArray(m_treeObjects[i]->getChunk()->getDebugVAO());
					glEnableVertexAttribArray(0);
					glDrawArrays(GL_LINES, 0, 24);
					glDisableVertexAttribArray(0);
					glBindVertexArray(0);
				}
				else if (m_treeObjects[i]->getLight() != nullptr)
				{
					/*	shader->setColor(glm::vec3(0.42, 0.25f, 0.75f));
						glBindVertexArray(m_treeObjects[i]->getLight()->getVAO());
						glEnableVertexAttribArray(0);
						glDrawArrays(GL_LINES, 0, 24);
						glDisableVertexAttribArray(0);
						glBindVertexArray(0);*/
				}
			}


		}
		


		if (m_northWest == nullptr)
			return;

		m_northWest->draw(shader);
		m_northEast->draw(shader);
		m_southWest->draw(shader);
		m_southEast->draw(shader);

	}
}
