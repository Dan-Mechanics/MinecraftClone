#include "ThreadPool.h"
#include "Game.h"
#include "FastNoiseLite.h"
#include "minecraft_clone.h"

constexpr auto WIDTH = 1920u;
constexpr auto HEIGHT = 1080u;
constexpr auto FULLSCREEN = false;

constexpr auto FRAMERATE_LIMIT = 300u;
constexpr auto FRAME_INTERVAL = 1.0f / FRAMERATE_LIMIT;

constexpr auto TICKRATE = 50u;
constexpr auto TICK_INTERVAL = 1.0f / TICKRATE;

auto hasFocus = true;
void focusCallback(GLFWwindow* window, int focus) {
	hasFocus = focus;
}

auto scrollInput = 0;
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
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
void errorCallback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft Clone", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowFocusCallback(window, &focusCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	if (FULLSCREEN)
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, WIDTH, HEIGHT, 144);

	glfwSetScrollCallback(window, scrollCallback);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, WIDTH, HEIGHT);

	// ===

	Game game{ WIDTH, HEIGHT };
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

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// https://stackoverflow.com/questions/57800608/how-to-render-at-a-fixed-fps-in-a-glfw-window
	auto previous = glfwGetTime();
	auto lastUpdateTime = glfwGetTime();
	auto updateTimer = 0.0;
	auto tickTimer = 0.0f;

	// THIS HAS A BIG IMPACT ON PERFORMANCE.
	// glfwSwapInterval(0);
	while (!glfwWindowShouldClose(window)) {
		const auto current = glfwGetTime();
		updateTimer += current - previous;
		previous = current;

		glfwPollEvents();
		if (updateTimer < FRAME_INTERVAL)
			continue;

		const auto deltaTime = static_cast<float>(current - lastUpdateTime);
		lastUpdateTime = current;
		updateTimer = 0.0f;

		const auto title = "fps: " + std::to_string(round(1.0f / deltaTime));
		glfwSetWindowTitle(window, title.c_str());

		// UPDATE. ===
		game.update(deltaTime, hasFocus, scrollInput, window, pool);

		// FIXED UPDATE. ===
		tickTimer += deltaTime;
		while (tickTimer >= TICK_INTERVAL) {
			tickTimer -= TICK_INTERVAL;
			game.tick(TICK_INTERVAL, pool, window);
		}

		// RENDER SHADOWS. ===
		glEnable(GL_DEPTH_TEST);
		game.drawShadows(deltaTime, hasFocus, window);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
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
	}

	game.free();
	pool.shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}