#include "MasterRenderer.h"
#include "Utility/MatrixCreator.h"
#include "App/AppSettings.h"
#include <iostream>
MasterRenderer::MasterRenderer(FPSCamera* camera)
{
	m_camera = camera;
	m_activeCamera = camera;
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_regularRenderer = new RegularRenderer(camera->getProjectionMatrix());
	m_instancedRenderer = new InstancedRenderer(camera->getProjectionMatrix());
	m_terrainRenderer = new TerrainRenderer(camera->getProjectionMatrix());

	m_FBO = new FBO();
	m_FBO->genFrameBuffer();
	
	m_fboQuad = new Quad();
	m_fboQuad->create();
	
	m_fboShader = new FBOShader();

	m_sunPosition = glm::vec3(0.f);

	m_quadTreeDebugShader = new QuadTreeDebugShader();
	m_quadTreeDebugShader->use();
	m_quadTreeDebugShader->setProjectionMatrix(camera->getProjectionMatrix());
	m_quadTreeDebugShader->unuse();

	m_lights.reserve(AppSettings::MAXLIGHTS());

	m_lightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f, -1.0f, 30.f);
	m_sunDirection = glm::vec3(24.f, 0.f, 43.f);
	m_shadowMapShader = new ShadowMapShader();

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
	delete m_shadowMapShader;
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

void MasterRenderer::passAllEntitiesForShadowMapping(std::vector<Entity*>* allEntities)
{
	this->m_regularRenderer->passAllEntitiesForShadowMapping(allEntities);
}

void MasterRenderer::render()
{
	// Shadow mapping pass
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	m_lightView = glm::lookAt(m_sunPosition,  m_sunDirection, glm::vec3(0.f, 1.0f, 0.f));
	lightTransform = m_lightProj * m_lightView;

	m_shadowMapShader->use();
	m_shadowMapShader->setLightTransform(lightTransform);

	glViewport(0, 0, AppSettings::SHADOW_WIDTH(), AppSettings::SHADOW_HEIGHT());
	m_FBO->bindShadowFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	m_terrainRenderer->shadowMapPass(m_shadowMapShader);
	m_regularRenderer->shadowMapPass(m_shadowMapShader);
	m_FBO->unbindShadowFramebuffer();
	m_shadowMapShader->unuse();
	//----------------------------
	glDisable(GL_CULL_FACE);

	// Render to framebuffer ( Deferred )
	glViewport(0, 0, AppSettings::SRCWIDTH(), AppSettings::SRCHEIGHT());
	m_FBO->bindDeferredFramebuffer();
	glClearColor(0.f,0.f,0.f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (AppSettings::DEBUG_LAYER())
		renderQuadTree();

	m_regularRenderer->render(m_activeCamera, m_camera);
	m_instancedRenderer->render(m_activeCamera, m_camera);
	m_terrainRenderer->render(m_activeCamera, m_camera);
	
	m_FBO->unbindDeferredFramebuffer();
	//-------------------------------------
	
	// Render the scene texture to a quad
	renderFBO();

	m_lights.clear();

	// Copy over the depthbuffer from the previous passes.
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO->gBuffer());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
	glBlitFramebuffer(
		0, 0, AppSettings::SRCWIDTH(), AppSettings::SRCHEIGHT(), 0, 0, AppSettings::SRCWIDTH(), AppSettings::SRCHEIGHT(), GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


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
	m_FBO->bindDeferredTextures();
	m_fboShader->registerLights(&m_lights);
	m_fboShader->setSunPosition(m_sunPosition);
	m_fboShader->setCameraPosition(m_activeCamera->getPosition());
	m_fboShader->setLightTransform(lightTransform);
	m_fboQuad->bind();
	glDrawArrays(GL_TRIANGLES, 0, m_fboQuad->getNumVertices());
	m_FBO->unbindDeferredTextures();
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


