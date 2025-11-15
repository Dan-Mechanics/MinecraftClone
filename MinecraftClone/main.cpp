// https://youtu.be/z03LXhRBLGI?si=kGOiI6dm8b5YW3Lo

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Texture.h"
#include "shaderClass.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"

// WE ARE MAKING A TRIANGLE.
// LET'S FUCKING GOOOO !!
GLfloat vertices[] =
{ 
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, 
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  
};

GLuint indices[] =
{
	0, 2, 1, 
	0, 3, 2 
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

	vao1.linkAttribute(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.linkAttribute(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.linkAttribute(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	GLuint uniformId = glGetUniformLocation(shaderProgram.id, "scale");

	Texture bluebells("texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	bluebells.setTextureUnit(shaderProgram, "tex0", 0);

	while (!glfwWindowShouldClose(window)) {
		// BACKGROUND COLOR	
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.activate();
		glUniform1f(uniformId, 0.5f);
		bluebells.bind();
		vao1.bind(); //				 COUNT OF INDICES.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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