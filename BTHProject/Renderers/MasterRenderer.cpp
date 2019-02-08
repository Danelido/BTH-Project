#include "MasterRenderer.h"
#include "Utility/MatrixCreator.h"
#include "App/AppSettings.h"

MasterRenderer::MasterRenderer(FPSCamera* camera)
{
	m_camera = camera;
	m_activeCamera = camera;
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	m_projectionMatrix = MatrixCreator::createNewProjectionMatrix(m_camera->FOV, (float)AppSettings::SRCWIDTH(), (float)AppSettings::SRCHEIGHT(), m_camera->NEAR_CLIPPING, m_camera->FAR_CLIPPING);
	
	m_regularRenderer = new RegularRenderer(m_projectionMatrix);
	m_instancedRenderer = new InstancedRenderer(m_projectionMatrix);
	m_terrainRenderer = new TerrainRenderer(m_projectionMatrix);

	m_FBO = new FBO();
	m_FBO->genFrameBuffer();
	
	m_fboQuad = new Quad();
	m_fboQuad->create();
	
	m_fboShader = new FBOShader();

	m_sunPosition = glm::vec3(-1.f, 15.f, -1.f);

	m_quadTreeDebugShader = new QuadTreeDebugShader();
	m_quadTreeDebugShader->use();
	m_quadTreeDebugShader->setProjectionMatrix(m_projectionMatrix);
	m_quadTreeDebugShader->unuse();

	m_lights.reserve(AppSettings::MAXLIGHTS());
}


MasterRenderer::~MasterRenderer()
{
	delete m_regularRenderer;
	delete m_instancedRenderer;
	delete m_terrainRenderer;
	delete m_FBO;
	delete m_fboQuad;
	delete m_fboShader;
	delete m_quadTreeDebugShader;
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

void MasterRenderer::submitTerrain(TerrainChunk * chunk)
{
	m_terrainRenderer->submit(chunk);
}

void MasterRenderer::registerQuadTree(QuadTree * quadTree)
{
	m_quadTree = quadTree;
}

void MasterRenderer::render()
{
	m_FBO->bindFramebuffer();
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (AppSettings::DEBUG_LAYER())
		renderQuadTree();

	m_regularRenderer->render(m_activeCamera);
	m_instancedRenderer->render(m_activeCamera, m_camera);
	m_terrainRenderer->render(m_activeCamera, m_camera);
	
	m_FBO->unbindFramebuffer();

	renderFBO();

	m_lights.clear();


	

}

void MasterRenderer::changeCamera(FPSCamera * camera)
{
	m_activeCamera = camera;
}

void MasterRenderer::renderFBO()
{
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	m_fboShader->use();
	m_FBO->bindTexture();
	m_fboShader->registerLights(&m_lights);
	m_fboShader->setSunPosition(m_sunPosition);
	m_fboShader->setCameraPosition(m_activeCamera->getPosition());
	m_fboQuad->bind();
	glDrawArrays(GL_TRIANGLES, 0, m_fboQuad->getNumVertices());
	m_FBO->unbindTexture();
	m_fboQuad->unbind();
	m_fboShader->unuse();
}

void MasterRenderer::renderQuadTree()
{
	if (m_quadTree) {
		m_quadTreeDebugShader->use();
		m_quadTreeDebugShader->setViewMatrix(m_activeCamera->getViewMatrix());
		m_quadTreeDebugShader->setModelMatrix(glm::mat4(1.f));
		m_camera->debugDraw();
		m_quadTree->draw(m_quadTreeDebugShader);
		m_quadTreeDebugShader->setColor(glm::vec3(0.2f, 0.3, 1.0f));
		m_quadTreeDebugShader->unuse();
	}
}

void MasterRenderer::setSunPosition(const glm::vec3& position)
{
	m_sunPosition = position;
}

