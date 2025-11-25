#include "Cube.h"

Cube::Cube() = default;
Cube::Cube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : position{ position }, rotation{ rotation }, scale{ scale } {
	std::vector <Vertex> verts(cubeVerticies, cubeVerticies + sizeof(cubeVerticies) / sizeof(Vertex));
	std::vector <GLuint> tris(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));
	std::vector<Texture> tex{};
	Mesh cube{ verts, tris, tex };
}

void Cube::draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor) const {
	shader.activate();
	mesh.vao.bind();
	 
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");

	// Initialize matrices
	glm::mat4 model = glm::identity<glm::mat4>(); // DO NOTHING
	glm::mat4 trans = glm::identity<glm::mat4>();
	glm::mat4 rot = glm::identity<glm::mat4>(); // TODO
	glm::mat4 sca = glm::identity<glm::mat4>();

	// Transform the matrices to their correct form
	trans = glm::translate(trans, position);
	rot = glm::mat4_cast(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)); // TODO
	sca = glm::scale(sca, scale);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// SEND LIGHT DATA
	glUniform3f(glGetUniformLocation(shader.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Cube::move(const glm::vec3& vel, const double dt) {
	position += vel * (float)dt;
}

void Cube::free() const {
	mesh.free();
}