#include "Mesh.h"
#include "Cube.h"
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

	//GLFWwindow* window = glfwCreateWindow(width, height, "My Title", glfwGetPrimaryMonitor(), NULL);
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

	glm::vec4 skyColor = glm::vec4((float)110 / 255, (float)164 / 255, (float)230 / 255, 1.0f);
	glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// glm::vec3 lightPos{ 0.5f, 0.5f, 0.5f };

	// ===

	Shader defaultShader("default.vert", "default.frag");

	std::vector<Vertex> pyramidVerts{};
	std::vector<GLuint> pyramidTris{};
	getPyramid(pyramidVerts, pyramidTris);

	std::vector <Texture> textures = {
		Texture("planks.png", "diffuse", 0),
		Texture("_planksSpec.png", "specular", 1)
	};

	Mesh pyramid(pyramidVerts, pyramidTris, textures);
	Mesh pyramid2(pyramidVerts, pyramidTris, textures);
	
	glm::vec3 pyramidPivot{};
	glm::mat4 pyramidMatrix = glm::mat4{ 1.0f };
	pyramidMatrix = glm::translate(pyramidMatrix, pyramidPivot);
	glm::mat4 pyramidMatrix2 = glm::translate(pyramidMatrix, glm::vec3{0.0f, 2.0f, 0.0f});

	//glm::vec3 cubePivot{};
	//glm::mat4 cubeMatrix = glm::translate(glm::mat4{ 1.0f }, pyramidPivot);
	//	glm::mat4 cubeMatrix = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ -0.5f, -0.5f, -0.5f });

	// ===

	//Shader cubeShader("default.vert", "default.frag");
	//Shader sunShader("default.vert", "unlit_color.frag");

	std::vector<Vertex> cubeVerts{};
	std::vector<GLuint> cubeTris{};
	getCube(cubeVerts, cubeTris);

	//Mesh cube1(cubeVerts, cubeTris, textures);
	//Mesh cube2(cubeVerts, cubeTris, textures);

	Cube sunCube (cubeVerts, cubeTris, textures, glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{0.0f} , glm::vec3{0.25f});
	Cube ground  (cubeVerts, cubeTris, textures, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{100.0f, 1.0f, 100.0f} );
	Cube redCube (cubeVerts, cubeTris, textures, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{0.0f}, glm::vec3{0.5f} );
	Cube blueCube(cubeVerts, cubeTris, textures, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{0.51f} );

	// ===

	glEnable(GL_DEPTH_TEST);

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/

	// METERS PER SECOND.
	const auto standardSpeed = 1.0f;
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
			/*sunColor.r += (float)tickInterval * (increase ? 1.0f : -1.0f);
			if (sunColor.r > 1.0f) {
				sunColor.r = 1.0f;
				increase = !increase;
			}

			if (sunColor.r < 0.0f) {
				sunColor.r = 0.0f;
				increase = !increase;
			}

			sunColor.g = sunColor.r;
			sunCube.setColor(sunColor);*/

			//redCube.rot.y += 1.0f;
			//blueCube.rot.y -= 1.0f;
		}

		camera.hasFocus = hasFocus;
		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(103.0f, 0.01f, 100.0f);

		pyramid.draw(defaultShader, camera, pyramidMatrix, glm::vec3{ 0.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, sunCube.pos, sunColor, skyColor);

		pyramid2.draw(defaultShader, camera, pyramidMatrix2, glm::vec3{ 0.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, sunCube.pos, sunColor, skyColor);

		/*cube1.draw(defaultShader, camera, pyramidMatrix, glm::vec3{ 0.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, sunCube.pos, sunColor, skyColor);

		cube2.draw(defaultShader, camera, pyramidMatrix, glm::vec3{ 2.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, sunCube.pos, sunColor, skyColor);*/
		ground.draw(defaultShader, camera, sunColor, sunCube.pos, skyColor);
		sunCube.draw(defaultShader, camera, sunColor, sunCube.pos, skyColor);
		redCube.draw(defaultShader, camera, sunColor, sunCube.pos, skyColor);
		blueCube.draw(defaultShader, camera, sunColor, sunCube.pos, skyColor);

		//cube.draw(cubeShader, )

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