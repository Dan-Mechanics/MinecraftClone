// https://youtu.be/z03LXhRBLGI?si=kGOiI6dm8b5YW3Lo

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"

// WE ARE MAKING A TRIANGLE.
// LET'S FUCKING GOOOO !!
GLfloat vertices[] = {
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,

	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,
};

GLuint indices[] = {
	0, 3, 5,
	3, 2, 4,
	5, 4, 1
};

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // THIS SHOULD BE 4, BUT THIS WORKS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const auto w = 800;
	const auto h = 800; //									    FULLSCREEN
	GLFWwindow* window = glfwCreateWindow(w, h, "Minecraft Clone", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, w, h);

	Shader shaderProgram{ "default.vert", "default.frag" };

	vao vao1;
	vao1.bind();
	
	vbo vbo1{ vertices, sizeof(vertices) };
	ebo ebo1{ indices, sizeof(indices) };

	vao1.linkVbo(vbo1, 0);
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	while (!glfwWindowShouldClose(window)) {
		// BACKGROUND COLOR	
		glClearColor(0.06f, 0.07f, 0.16f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.activate();
		vao1.bind(); //				 COUNT OF INDICES.
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao1.free();
	vbo1.free();
	ebo1.free();
	shaderProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}