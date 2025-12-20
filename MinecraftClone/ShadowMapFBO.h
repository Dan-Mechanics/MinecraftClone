#ifndef SHADOW_MAP_CLASS_FBO_H
#define SHADOW_MAP_CLASS_FBO_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Object.h"

class ShadowMapFBO {
public:
	unsigned int shadowMapWidth{};
	unsigned int shadowMapHeight{};

	unsigned int shadowMapFBO{};
	unsigned int shadowMap{};

	glm::mat4 lightProjection{};
	float dist{};

	ShadowMapFBO();
	ShadowMapFBO(const unsigned int shadowMapWidth, const unsigned int shadowMapHeight, const float dist);

	void activate(const Camera& camera, const Object& sun);
	void bind(const Shader& shader) const;
	void sendToShader(const Shader& shader) const;
	void free() const;
};

#endif