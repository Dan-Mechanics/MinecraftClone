#include "Mesh.h"
#include "Object.h"
#include "utils.h"

const unsigned int width = 1920;
const unsigned int height = 1080;
const double fpsCap = 300.0;
const double minDtForFrame = 1.0 / fpsCap;
const double tickInterval = 0.02;

bool hasFocus = true;
static void setFocus(GLFWwindow* window, int focus) {
	hasFocus = focus;
}

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // THIS SHOULD BE 4, BUT THIS WORKS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft Clone", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowFocusCallback(window, &setFocus);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, 144);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// ===

	glm::vec4 skyColor = glm::vec4((float)50 / 255, (float)50 / 255, (float)100 / 255, 1.0f);
	glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// ===

	Shader defaultShader("default.vert", "default.frag");

	std::vector<Vertex> pyramidVerts{};
	std::vector<GLuint> pyramidTris{};
	getPyramid(pyramidVerts, pyramidTris);

	std::vector <Texture> wood = {
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};

	std::vector <Texture> grass = {
		Texture("albedo.png", "diffuse", 0),
		Texture("specular.png", "specular", 1)
	};

	Mesh pyramid{ pyramidVerts, pyramidTris, wood };
	glm::mat4 pyramidMatrix = glm::mat4{ 1.0f };

	// ===

	Shader textureLit("default.vert", "worldlight.frag");
	Shader litShader("default.vert", "lit_color.frag");
	Shader unlitShader("default.vert", "unlit_color.frag");

	std::vector<Vertex> cubeVerts{};
	std::vector<GLuint> cubeTris{};
	getCube(cubeVerts, cubeTris);

	Mesh cube{ cubeVerts, cubeTris, grass };
	Mesh groundMesh{ cubeVerts, cubeTris, wood };

	Object sunCube{ glm::vec3{0.0f, 25.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{0.25f} };
	sunCube.setColor(sunColor);

	Object ground{ glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{100.0f, 1.0f, 100.0f} };

	Object redCube{ glm::vec3{2.0f, 0.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{1.0f} };
	redCube.setColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	Object greenCube{ glm::vec3{0.0f, 2.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{1.0f} };
	greenCube.setColor(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	Object blueCube{ glm::vec3{0.0f, 0.0f, 2.0f}, glm::vec3{0.0f}, glm::vec3{1.0f} };
	blueCube.setColor(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	// ===

	glEnable(GL_DEPTH_TEST);

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// METERS PER SECOND.
	const auto standardSpeed = 12.5f;
	const auto sensitivity = 0.1f;
	Camera camera{ window, width, height, standardSpeed, sensitivity };

	double previousTime = 0.0;
	double currentTime = 0.0;
	double timer = 0.0;
	bool increase = false;

	// DISABLE VSYNC.
	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		if (deltaTime < minDtForFrame)
			continue;

		previousTime = currentTime;

		std::string fps = std::to_string(1.0 / deltaTime);
		std::string ms = std::to_string(deltaTime * 1000);
		std::string newTitle = "fps: " + fps + " | ms: " + ms;
		glfwSetWindowTitle(window, newTitle.c_str());

		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;

			// ..
		}

		camera.hasFocus = hasFocus;
		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(103.0f, 0.01f, 100.0f);

		/*pyramid.draw(textureLit, camera, pyramidMatrix, glm::vec3{ 0.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, sunCube.pos, sunColor, skyColor);*/

		ground.draw(groundMesh, textureLit, camera, sunColor, sunCube.pos, skyColor);
		sunCube.drawColor(cube, unlitShader, camera, sunColor, sunCube.pos, skyColor);

		redCube.draw(cube, textureLit, camera, sunColor, sunCube.pos, skyColor);
		greenCube.draw(cube, textureLit, camera, sunColor, sunCube.pos, skyColor);
		blueCube.draw(cube, textureLit, camera, sunColor, sunCube.pos, skyColor);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			break;
	}

	pyramid.free();
	//sunCube.free();
	//defaultShader.free();
	defaultShader.free();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}