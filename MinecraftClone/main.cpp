#include "Mesh.h"
#include "Object.h"
#include "utils.h"
#include "world_mesh.h"
#include <unordered_map>
#include "BlockType.h"

const unsigned int width = 1920;
const unsigned int height = 1080;
const double fpsCap = 300.0;
const double minDtForFrame = 1.0 / fpsCap;
const double tickInterval = 0.02;

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
	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, 144);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// ===

	glm::vec4 skyColor = glm::vec4((float)90 / 255, (float)86 / 255, (float)150 / 255, 1.0f);
	glm::vec4 sunColor = glm::vec4((float)255 / 255, (float)255 / 255, (float)255 / 255, 1.0f);

	// ===

	Shader materialShader("default.vert", "material.frag");
	Shader unlitShader("default.vert", "unlit_color.frag");

	// ===

	std::vector <Texture> woodMaterial = {
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};

	std::vector <Texture> atlasMaterial = {
		Texture("texture_atlas.png", "diffuse", 0),
		Texture("texture_atlas_specular.png", "specular", 1)
	};

	// ===

	std::vector<Vertex> cubeVerts{};
	std::vector<GLuint> cubeTris{};
	glm::mat4 cubeMatrix;
	getCubeMesh(cubeVerts, cubeTris, cubeMatrix);

	Mesh cubeMesh{ cubeVerts, cubeTris, cubeMatrix };

	// ===

	const auto atlas = generateAtlas();
	const auto world = generateWorld();

	std::vector<Vertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};
	glm::mat4 chunkMatrix;

	getChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, world.at({ 0, 0, 0 }), world);

	Mesh chunkMesh{ chunkVerts, chunkTris, chunkMatrix };

	// ===

	Object sun{ glm::vec3{8.0f, 12.0f, 8.0f}, glm::vec3{0.0f}, glm::vec3{0.25f} };
	sun.setColor(sunColor);

	Object ground{ glm::vec3{0.0f, -3.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{100.0f, 1.0f, 100.0f} };

	// HERE YOU CAN CHANGE THE LOOK OF THE CHUNK.
	Object chunkObject{ glm::vec3{ 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };

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

	// DISABLE VSYNC.
	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		// MAYBE USE THREAD.SLEEP FOR THIS?
		// SINCE WE WANT TO AVOID BUSY WAITING ...
		// https://discourse.glfw.org/t/frame-limiting/70/4
		// MAYBE THAT'S FINE IDK
		if (deltaTime < minDtForFrame)
			continue;

		previousTime = currentTime;

		std::string fps = std::to_string(1.0 / deltaTime);
		std::string ms = std::to_string(deltaTime * 1000);
		std::string newTitle = "fps: " + fps + " | ms: " + ms;
		glfwSetWindowTitle(window, newTitle.c_str());

		// ===

		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			// ...
		}

		camera.hasFocus = hasFocus;
		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(103.0f, 0.01f, 100.0f);

		ground.drawWithMaterial(cubeMesh, woodMaterial, materialShader, camera, sunColor, sun.pos, skyColor);
		sun.drawAsUnlitColor(cubeMesh, unlitShader, camera);

		chunkObject.drawWithMaterial(chunkMesh, atlasMaterial, materialShader, camera, sunColor, sun.pos, skyColor);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	materialShader.free();
	unlitShader.free();

	freeMaterial(woodMaterial);
	freeMaterial(atlasMaterial);

	cubeMesh.free();
	chunkMesh.free();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}