#include "ThreadPool.h"
#include "Game.h"
#include <chrono>
#include <thread>
#include <iostream>
#include "../World/FastNoiseLite.h"

const unsigned int width = 1920;
const unsigned int height = 1080;
bool hasFocus = true;
auto scrollInput = 0;

const auto maxFps = 300.0f;
const auto frameInterval = 1.0f / maxFps;
const auto tickInterval = 0.02f;

static void focusCallback(GLFWwindow* window, int focus) {
	hasFocus = focus;
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	if (yOffset > 0) {
		scrollInput++;
	}
	else if (yOffset < 0) {
		scrollInput--;
	}
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

	glfwSetScrollCallback(window, scrollCallback);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// ===

	Game game{ width, height };
	game.setup(window);

	// ===

	glm::vec4 clearColor = game.getClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	// ===

	ThreadPool pool{ 16 };
	pool.init();

	// ===

	//glEnable(GL_DEPTH_TEST);

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		// UPDATE. ===
		game.update(deltaTime, hasFocus, scrollInput, window, pool);

		// FIXED UPDATE. ===
		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			game.tick(tickInterval, pool, window);
		}

		// RENDER SHADOWS. ===
		glEnable(GL_DEPTH_TEST);
		game.drawShadows(deltaTime, hasFocus, window);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// RENDER. ===
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		game.draw();

		// https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%2017%20-%20Transparency%20%26%20Blending/Main.cpp
		glEnable(GL_BLEND);
		game.drawTranslucent();
		glDisable(GL_BLEND);

		// UI. ===
		glDisable(GL_DEPTH_TEST);
		game.drawUI(deltaTime, hasFocus, window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	game.free();
	pool.shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}