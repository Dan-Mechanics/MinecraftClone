// https://youtu.be/z03LXhRBLGI?si=kGOiI6dm8b5YW3Lo

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "shaderClass.h"
#include "vbo.h"
#include "vao.h"
#include "ebo.h"

// WE ARE MAKING A TRIANGLE.
// LET'S FUCKING GOOOO !!
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.02f,	0.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f
};

GLuint indices[] = {
	0, 2, 1,
	0, 3, 2,
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
	vao1.linkAttribute(vbo1, 3, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	GLuint uniformId = glGetUniformLocation(shaderProgram.id, "scale");

	int widthImg, heightImg, numColorChannels;
	unsigned char* bytes = stbi_load("texture.png", &widthImg, &heightImg, &numColorChannels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0uni = glGetUniformLocation(shaderProgram.id, "tex0");
	shaderProgram.activate();
	glUniform1f(tex0uni, 0);

	while (!glfwWindowShouldClose(window)) {
		// BACKGROUND COLOR	
		glClearColor(0.06f, 0.07f, 0.16f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.activate();
		glUniform1f(uniformId, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);
		vao1.bind(); //				 COUNT OF INDICES.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao1.free();
	vbo1.free();
	ebo1.free();
	glDeleteTextures(1, &texture);
	shaderProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}