// https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

#include "Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Takes care of the information needed to draw the windows
const unsigned int numWindows = 100;
glm::vec3 positionsWin[numWindows];
float rotationsWin[numWindows];

// Takes care of drawing the windows in the right order
unsigned int orderDraw[numWindows];
float distanceCamera[numWindows];

// Compare function
int compare(const void* a, const void* b)
{
	double diff = distanceCamera[*(int*)b] - distanceCamera[*(int*)a];
	return  (0 < diff) - (diff < 0);
}

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
	Shader grassProgram("default.vert", "grass.frag");
	Shader winProgram("default.vert", "windows.frag");

	// ===

	glm::vec4 lightColor = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 lightPos = glm::vec3{ 0.5f, 0.5f, 0.5f };
	glm::mat4 lightModel = glm::mat4{ 1.0f };
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.activate();
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	grassProgram.activate();
	glUniform4f(glGetUniformLocation(grassProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(grassProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 
	glFrontFace(GL_CCW); // --> this depends on the model ur loading LOLOLOL.

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera camera{ width, height, glm::vec3{0.0f, 0.0f, 2.0f} };

	Model ground{ "models/ground/scene.gltf" };
	Model grass{ "models/grass/scene.gltf" };
	Model windows{ "models/windows/scene.gltf" };

	double fpsCap = 300.0;
	double minDeltaTimeForFrame = 1.0 / fpsCap;
	const float tickInterval = 0.01f;
	float timer = 0.0f;

	double prevTime = 0.0;
	double crntTime = 0.0;
	double deltaTime;
	unsigned int counter = 0;

	for (unsigned int i = 0; i < numWindows; i++)
	{
		positionsWin[i] = glm::vec3
		(
			-15.0f + static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / (15.0f - (-15.0f)))),
			1.0f + static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / (4.0f - 1.0f))),
			-15.0f + static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / (15.0f - (-15.0f))))
		);
		rotationsWin[i] = static_cast <float>(rand()) / (static_cast <float>(RAND_MAX / 1.0f));
		orderDraw[i] = i;
	}

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

		// BACKGROUND COLOR	
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(0.85f, 0.85f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timer += (float)deltaTime;
		while (timer > tickInterval) {
			timer -= tickInterval;
			camera.moveCamera(window, tickInterval);
			camera.updateMatrix(45.0f, 0.1f, 100.0f);
		}

		ground.draw(shaderProgram, camera);

		// Disable cull face so that grass and windows have both faces
		glDisable(GL_CULL_FACE);
		grass.draw(grassProgram, camera);
		// Enable blending for windows
		glEnable(GL_BLEND);
		// Get distance from each window to the camera
		for (unsigned int i = 0; i < numWindows; i++)
		{
			distanceCamera[i] = glm::length(camera.position - positionsWin[i]);
		}
		// Sort windows by distance from camera
		qsort(orderDraw, numWindows, sizeof(unsigned int), compare);
		// Draw windows
		for (unsigned int i = 0; i < numWindows; i++)
		{
			windows.draw(winProgram, camera, positionsWin[orderDraw[i]], glm::quat(1.0f, 0.0f, rotationsWin[orderDraw[i]], 0.0f));
		}
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	grass.free();
	ground.free();
	windows.free();
	shaderProgram.free();
	grassProgram.free();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}