// https://youtu.be/z03LXhRBLGI?si=kGOiI6dm8b5YW3Lo

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

// WE ARE MAKING A CUBE.
GLfloat vertices[] =
{ 
	-0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f, 
	-0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,     5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,     0.0f, 0.0f,
	 0.5f, 0.0f, 0.5f,     0.83f, 0.70f, 1.44f,     5.0f, 0.0f,
	 0.0f, 0.8f, 0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f  
};

GLuint indices[] =
{
	0, 1, 2, 
	0, 2, 3, 
	0, 1, 4, 
	1, 2, 4, 
	2, 3, 4, 
	3, 0, 4
};

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // THIS SHOULD BE 4, BUT THIS WORKS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// HERE YOU CAN SET FULLSCREEN OR NOT.
	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft Clone", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	Shader shaderProgram{ "default.vert", "default.frag" };

	vao vao1;
	vao1.bind();
	
	vbo vbo1{ vertices, sizeof(vertices) };
	ebo ebo1{ indices, sizeof(indices) };

	vao1.linkAttribute(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.linkAttribute(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.linkAttribute(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	Texture bluebells("texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	bluebells.setTextureUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera{ width, height, glm::vec3{0.0f, 0.0f, 2.0f} };

	while (!glfwWindowShouldClose(window)) {
		// BACKGROUND COLOR	
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.activate();

		camera.moveCamera(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		camera.applyMatrix(shaderProgram, "camMatrix");

		bluebells.bind();
		vao1.bind(); //				 COUNT OF INDICES.
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao1.free();
	vbo1.free();
	ebo1.free();
	bluebells.free();
	shaderProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}