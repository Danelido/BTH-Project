#include "FBOShader.h"
#include "App/AppSettings.h"
FBOShader::FBOShader()
{
	if (!loadShaders("Resources/Shaders/FBO.vert", "Resources/Shaders/FBO.frag", nullptr))
	{
		system("pause");
	}
	use();

	m_cameraPositionLocation = glGetUniformLocation(program(), "cameraPos");
	m_numberOfLightsLocation = glGetUniformLocation(program(), "nrOfLights");
	m_sunPositionLocation = glGetUniformLocation(program(), "sunPosition");

	for (int i = 0; i < AppSettings::MAXLIGHTS(); i++)
	{
		std::string posLoc = "lights[" + std::to_string(i) + "].position";
		std::string colLoc = "lights[" + std::to_string(i) + "].color";

		lightShaderProperties block;
		block.positionLocation = glGetUniformLocation(this->program(), posLoc.c_str());
		block.colorLocation = glGetUniformLocation(this->program(), colLoc.c_str());

		m_lightShaderProperties.emplace_back(block);
	}

	glUniform1i(glGetUniformLocation(program(), "gPosition"),0);
	glUniform1i(glGetUniformLocation(program(), "gNormal"), 1);
	glUniform1i(glGetUniformLocation(program(), "gAlbedoSpec"), 2);

	unuse();
}


FBOShader::~FBOShader()
{
}

void FBOShader::setCameraPosition(const glm::vec3& position)
{
	glUniform3f(m_cameraPositionLocation, position.x, position.y, position.z);
}

void FBOShader::setSunPosition(const glm::vec3& position)
{
	glUniform3f(m_sunPositionLocation, position.x, position.y, position.z);
}

void FBOShader::registerLights(const std::vector<Light*>* lights)
{
	
	glUniform1i(m_numberOfLightsLocation, (int)(lights->size()));

	for (int i = 0; i < lights->size(); i++)
	{
		glUniform3f(m_lightShaderProperties[i].positionLocation,
			lights->at(i)->getPosition().x,
			lights->at(i)->getPosition().y,
			lights->at(i)->getPosition().z);

		glUniform3f(m_lightShaderProperties[i].colorLocation,
			lights->at(i)->getColor().x,
			lights->at(i)->getColor().y,
			lights->at(i)->getColor().z);
	}
	
}
