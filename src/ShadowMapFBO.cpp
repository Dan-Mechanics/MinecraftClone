#include "ShadowMapFBO.h"

ShadowMapFBO::ShadowMapFBO() = default;
ShadowMapFBO::ShadowMapFBO(const unsigned int shadowMapWidth, const unsigned int shadowMapHeight, const float dist) :
	shadowMapWidth{ shadowMapWidth }, shadowMapHeight{ shadowMapHeight }, dist{ dist } {
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "shadow framebuffer broken!!" << std::endl;
		exit(EXIT_FAILURE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapFBO::activate(const Camera& camera, const Object& sun) {
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 orthgonalProjection = glm::ortho(-dist, dist, -dist, dist, -dist, dist);
	glm::mat4 lightView = glm::lookAt(glm::normalize(sun.pos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, -camera.position);
	lightProjection = orthgonalProjection * lightView * translation;
}

void ShadowMapFBO::bind(const Shader& shader) const {
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
}

void ShadowMapFBO::sendToShader(const Shader& shader) const {
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(shader.id, "shadowMap"), 2);
}

void ShadowMapFBO::free() const {
	glDeleteFramebuffers(1, &shadowMapFBO);
}