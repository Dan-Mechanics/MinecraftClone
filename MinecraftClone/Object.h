#pragma once
#include "Mesh.h"

class Object {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::vec4 color{};

	Object();
	Object(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

	void drawWithMaterial(const Mesh& mesh, const std::vector<Texture>& material,
		const Shader& shader, const Camera& camera, const glm::vec4& lightColor,
		const glm::vec3& lightPos, const glm::vec4& worldColor);

	void drawAsColor(const Mesh& mesh,const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	void drawAsUnlitColor(const Mesh& mesh, const Shader& shader, const Camera& camera);

	void move(const glm::vec3& vel, const float dt);
	void rotate(const glm::vec3& vel, const float dt);
	void setColor(const glm::vec4& color);

private:
	glm::quat calculateQuat();

};