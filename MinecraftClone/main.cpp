// https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

#include "Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
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

	// ===

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag", "default.geom");
	Shader normalsShader("default.vert", "normals.frag", "normals.geom");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.activate();
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// ===

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// ===

	Model model{ "models/statue/scene.gltf" };

	double fpsCap = 300.0;
	double minDeltaTimeForFrame = 1.0 / fpsCap;
	const float tickInterval = 0.01f;
	float timer = 0.0f;

	double prevTime = 0.0;
	double crntTime = 0.0;
	double deltaTime;
	unsigned int counter = 0;

	// DISABLE VSYNC
	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window)) {
		crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;

		if (deltaTime < minDeltaTimeForFrame)
			continue;

		++counter;
		std::string fps = std::to_string((1.0 / deltaTime) * counter);
		std::string ms = std::to_string((deltaTime / counter) * 1000);
		std::string newTitle = "fps: " + fps + " | ms: " + ms;
		glfwSetWindowTitle(window, newTitle.c_str());
		prevTime = crntTime;
		counter = 0;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timer += (float)deltaTime;
		while (timer > tickInterval) {
			timer -= tickInterval;
			camera.moveCamera(window, tickInterval);
			camera.updateMatrix(45.0f, 0.1f, 100.0f);
		}

		model.draw(shaderProgram, camera);
		model.draw(normalsShader, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	model.free();
	shaderProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}