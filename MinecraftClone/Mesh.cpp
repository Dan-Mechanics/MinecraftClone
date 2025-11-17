#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& verticies, std::vector<GLuint>& indices, std::vector<Texture>& textures) :
	vertices{ verticies }, indices{ indices }, textures{ textures } {
	vao.bind();

	vbo vbo{ vertices };
	ebo ebo{ indices };

	vao.linkAttribute(vbo, 0, 3, GL_FLOAT, 11 * sizeof(Vertex), (void*)0);
	vao.linkAttribute(vbo, 1, 3, GL_FLOAT, 11 * sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttribute(vbo, 2, 2, GL_FLOAT, 11 * sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttribute(vbo, 3, 3, GL_FLOAT, 11 * sizeof(Vertex), (void*)(9 * sizeof(float)));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	// free here ??
}

void Mesh::draw(Shader& shader, Camera& camera) {
	shader.activate();
	vao.bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular") {
			num = std::to_string(numSpecular++);
		}

		textures[i].setTextureUnit(shader, (type + num).c_str(), i);
		textures[i].bind();
	}

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.applyMatrix(shader, "camMatrix");

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}