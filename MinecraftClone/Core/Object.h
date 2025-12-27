#pragma once
#include "../Rendering/Mesh.h"

class Object {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::vec4 color{};
	bool visible{};

	Object();

	void drawWithMaterial(const Mesh& mesh, const std::vector<Texture>& material,
		const Shader& shader, const Camera& camera, const glm::vec4& lightColor,
		const glm::vec3& lightPos, const glm::vec4& worldColor);

	void drawAsColor(const Mesh& mesh,const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	void drawAsUnlitColor(const Mesh& mesh, const Shader& shader, const Camera& camera);

	void moveOverTime(const glm::vec3& vel, const float dt);
	void rotateOverTime(const glm::vec3 & angVel, const float dt);

	void setPos(const glm::vec3& to);
	void setRot(const glm::vec3& to);
	void setScale(const glm::vec3& to);
	void setColor(const glm::vec4& to);

private:
	glm::quat calculateQuat();

};