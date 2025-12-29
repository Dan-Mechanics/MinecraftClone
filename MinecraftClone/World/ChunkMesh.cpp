#include "ChunkMesh.h"

ChunkMesh::ChunkMesh() = default;
//ChunkMesh::ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices) {
//	this->vertices = vertices;
//	this->indices = indices;
//
//	vao.bind();
//	vbo = { vertices };
//	ebo = { indices };
//
//	vao.linkAttribute(vbo, 0, 3, GL_INT, sizeof(ChunkVertex), (void*)0);
//	vao.linkAttribute(vbo, 1, 3, GL_INT, sizeof(ChunkVertex), (void*)(3 * sizeof(int)));
//	vao.linkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(ChunkVertex), (void*)(6 * sizeof(int)));
//
//	vao.unbind();
//	vbo.unbind();
//	ebo.unbind();
//}

void ChunkMesh::bind(std::vector<ChunkVertex>& vertices, std::vector<GLuint>& indices)  {
	vao.bind();
	vbo = { vertices };
	ebo = { indices };

	triangleCount = indices.size();

	vao.linkAttribute(vbo, 0, 3, GL_INT, sizeof(ChunkVertex), (void*)0);
	vao.linkAttribute(vbo, 1, 3, GL_INT, sizeof(ChunkVertex), (void*)(3 * sizeof(int)));
	vao.linkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(ChunkVertex), (void*)(6 * sizeof(int)));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void ChunkMesh::drawShadows(const Shader& shader, const Camera& camera) const {
	shader.activate();
	vao.bind();

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");
	glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT, 0);
}

void ChunkMesh::draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor,
	const glm::vec4& worldColor, const std::vector<Texture>& material) const {
	shader.activate();
	vao.bind();

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");

	glUniform3f(glGetUniformLocation(shader.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform4f(glGetUniformLocation(shader.id, "worldColor"), worldColor.x, worldColor.y, worldColor.z, worldColor.w);

	glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT, 0);
}

void ChunkMesh::free() const {
	vao.free();
	vbo.free();
	ebo.free();
}