#include "Chunk.h"
#include "Mesh.h"
#include "Object.h"
#include "utils.h"
#include "world_mesh_utils.h"
#include <unordered_map>
#include "BlockType.h"
#include "World.h"
#include "ShadowMapFBO.h"
#include "mesh_utils.h"
#include "ThreadPool.h"
#include "Game.h"
#include <chrono>
#include <thread>

const float fpsCap = 300.0f;
const float minFrameInterval = 1.0f / fpsCap;
const unsigned int width = 1920;
const unsigned int height = 1080;
const float tickInterval = 0.02f;
bool hasFocus = true;

static void setFocus(GLFWwindow* window, int focus) {
	hasFocus = focus;
}

/// <summary>
/// https://www.glfw.org/docs/latest/quick.html
/// </summary>
static void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // THIS SHOULD BE 4, BUT THIS WORKS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft Clone", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowFocusCallback(window, &setFocus);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	// glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, 144);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// ===

	Game game{};
	game.setup(window, width, height);

	// ===

	glm::vec4 clearColor = game.getClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	// ===

	ThreadPool pool{ 16 };
	pool.init();

	// ===

	glEnable(GL_DEPTH_TEST);

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	const auto interval = std::chrono::nanoseconds((long long)floor(1.0f / 300.0f * 1000.0f * 1000000.0f));

	//float previousTime = 0.0f;
	//float currentTime = 0.0f;
	auto previous = std::chrono::high_resolution_clock::now();
	float timer = 0.0f;

	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window)) {
		//float currentTime = (float)glfwGetTime();
		const auto current = std::chrono::high_resolution_clock::now();
		const auto nanoDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(current - previous);
		auto deltaTime = nanoDuration.count() * 0.000001f; // MS.
		deltaTime *= 0.001f; // SECONDS.
		previous = current;

		const auto title = "fps: " + std::to_string(round(1.0f / deltaTime));
		glfwSetWindowTitle(window, title.c_str());

		// ===

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
				game.tick(tickInterval, pool);
		}

		game.drawShadows(deltaTime, hasFocus, window);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		game.draw(deltaTime, hasFocus, window);

		glfwSwapBuffers(window);
		glfwPollEvents();

		const auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current - endTime);
		duration = std::chrono::milliseconds((long long)floor(minFrameInterval * 1000.0f)) - duration;
		std::this_thread::sleep_for(duration);
	}

	game.free();
	pool.shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}