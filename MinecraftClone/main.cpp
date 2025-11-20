// https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

#include "Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

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

	// ===

	Shader shaderProgram("default.vert", "default.frag");

	Shader outliningProgram("outlining.vert", "outlining.frag");

	// ===

	glm::vec4 lightColor = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos = glm::vec3{ 0.5f, 0.5f, 0.5f };
	glm::mat4 lightModel = glm::mat4{ 1.0f };
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.activate();
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glDepthFunc(GL_LESS);

	Camera camera{ width, height, glm::vec3{0.0f, 0.0f, 2.0f} };

	Model trees{ "models/trees/scene.gltf" };
	Model ground{ "models/ground/scene.gltf" };

	int counter = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		counter++;
		if (counter > 300) {
			counter = 0;
		}
		else {
			continue;
		}

		// BACKGROUND COLOR	
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(0.85f, 0.85f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.moveCamera(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		trees.draw(shaderProgram, camera);
		ground.draw(shaderProgram, camera);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		outliningProgram.activate();
		glUniform1f(glGetUniformLocation(outliningProgram.id, "outlining"), 0.08f);

		trees.draw(outliningProgram, camera);
		ground.draw(outliningProgram, camera);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);

	}

	trees.free();
	ground.free();
	shaderProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}