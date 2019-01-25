#include "MasterRenderer.h"
#include "Utility/MatrixCreator.h"
#include "App/AppSettings.h"

MasterRenderer::MasterRenderer(const FPSCamera* camera)
{
	m_camera = camera;
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	m_projectionMatrix = MatrixCreator::createNewProjectionMatrix(45.f, (float)AppSettings::SRCWIDTH(), (float)AppSettings::SRCHEIGHT(), 0.1f, 300.f);
	
	m_regularRenderer = new RegularRenderer(m_projectionMatrix);
	m_instancedRenderer = new InstancedRenderer(m_projectionMatrix);
	m_terrainRenderer = new TerrainRenderer(m_projectionMatrix);

	m_FBO = new FBO();
	m_FBO->genFrameBuffer();
	
	m_fboQuad = new Quad();
	m_fboQuad->create();
	
	m_fboShader = new FBOShader();

	m_sunPosition = glm::vec3(-1.f, 15.f, -1.f);
}


MasterRenderer::~MasterRenderer()
{
	delete m_regularRenderer;
	delete m_instancedRenderer;
	delete m_terrainRenderer;
	delete m_FBO;
	delete m_fboQuad;
	delete m_fboShader;
	
	for (size_t i = 0; i < m_lights.size(); i++)
	{
		delete m_lights[i];
	}
}

void MasterRenderer::submitEntity(Entity * entity)
{
	if (entity->isInstanced())
	{
		m_instancedRenderer->submit(entity);
	}
	else
	{
		m_regularRenderer->submit(entity);
	}
}

void MasterRenderer::submitLight(Light * light)
{
	m_lights.emplace_back(light);
}

void MasterRenderer::submitTerrain(Terrain * terrain)
{
	m_terrainRenderer->submit(terrain);
}

void MasterRenderer::render()
{
	
	m_FBO->bindFramebuffer();
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_regularRenderer->render(m_camera);
	m_instancedRenderer->render(m_camera);
	m_terrainRenderer->render(m_camera);

	m_FBO->unbindFramebuffer();

	renderFBO();
}


void MasterRenderer::renderFBO()
{
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	m_fboShader->use();
	m_FBO->bindTexture();
	m_fboShader->registerLights(&m_lights);
	m_fboShader->setSunPosition(m_sunPosition);
	m_fboShader->setCameraPosition(m_camera->getPosition());
	m_fboQuad->bind();
	glDrawArrays(GL_TRIANGLES, 0, m_fboQuad->getNumVertices());
	m_FBO->unbindTexture();
	m_fboQuad->unbind();
	m_fboShader->unuse();
}


void MasterRenderer::setSunPosition(const glm::vec3& position)
{
	m_sunPosition = position;
}

const size_t& MasterRenderer::getNumberOfLights() const
{
	return m_lights.size();
}
