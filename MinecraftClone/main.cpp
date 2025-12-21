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
#include <iostream>

const unsigned int width = 1920;
const unsigned int height = 1080;
bool hasFocus = true;

const float maxFps = 300.0f;
const float frameInterval = 1.0f / maxFps;
const float tickInterval = 0.02f;

static void focusCallback(GLFWwindow* window, int focus) {
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
	glfwSetWindowFocusCallback(window, &focusCallback);

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

	glfwSwapInterval(0);
	auto previous = 0.0f;
	auto timer = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		const auto current = (float)glfwGetTime();
		const auto deltaTime = std::max(current - previous, 0.0f);

		if (deltaTime < frameInterval)
			continue;

		previous = current;
		const auto title = "fps: " + std::to_string(round(1.0f / deltaTime));
		glfwSetWindowTitle(window, title.c_str());

		// ===

		game.update(deltaTime, hasFocus, window, pool);

		// FIXED UPDATED.
		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			game.tick(tickInterval, pool, window);
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
	}

	game.free();
	pool.shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}