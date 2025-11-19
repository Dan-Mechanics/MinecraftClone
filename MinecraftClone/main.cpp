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

	/// ===

	Shader shaderProgram("default.vert", "default.frag");

	// ===

	glm::vec4 lightColor = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos = glm::vec3{ 0.5f, 0.5f, 0.5f };
	glm::mat4 lightModel = glm::mat4{ 1.0f };
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.activate();
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera{ width, height, glm::vec3{0.0f, 0.0f, 2.0f} };

	Model model{ "models/map/scene.gltf" };

	while (!glfwWindowShouldClose(window)) {
		// BACKGROUND COLOR	
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(0.1f, 0.12f, 0.18f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.moveCamera(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		model.draw(shaderProgram, camera);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	shaderProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}