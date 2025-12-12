#include "ChunkMesh.h"

ChunkMesh::ChunkMesh() = default;
ChunkMesh::ChunkMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
	this->vertices = vertices;
	this->indices = indices;

	vao.bind();
	vbo = { vertices };
	ebo = { indices };

	vao.linkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttribute(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

ChunkMesh::ChunkMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const glm::mat4& modelMatrix) {
	this->vertices = vertices;
	this->indices = indices;
	this->modelMatrix = modelMatrix;

	vao.bind();
	vbo = { vertices };
	ebo = { indices };

	vao.linkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttribute(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttribute(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void ChunkMesh::drawShadows(const Shader& shader, const Camera& camera) const {
	shader.activate();
	vao.bind();

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void ChunkMesh::draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor,
	const glm::vec4& worldColor, const std::vector<Texture>& material) const {
	shader.activate();
	vao.bind();

	auto diffuseCount = 0;
	auto specularCount = 0;
	for (int i = 0; i < material.size(); i++) {
		std::string num;
		std::string type = material[i].type;
		if (type == "diffuse") {
			num = std::to_string(diffuseCount++);
		}
		else if (type == "specular") {
			num = std::to_string(specularCount++);
		}

		// POSSIBLY ADD EMMISION IN THE FUTURE ??

		material[i].setTextureUnit(shader, (type + num).c_str(), i);
		material[i].bind();
	}

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");

	glUniform3f(glGetUniformLocation(shader.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform4f(glGetUniformLocation(shader.id, "worldColor"), worldColor.x, worldColor.y, worldColor.z, worldColor.w);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void ChunkMesh::free() const {
	vao.free();
	vbo.free();
	ebo.free();
}