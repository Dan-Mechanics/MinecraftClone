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

const unsigned int width = 1920;
const unsigned int height = 1080;
const float fpsCap = 300.0f;
const float minDeltaForFrame = 1.0f / fpsCap;
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
	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, 144);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// ===

	glm::vec4 sunColor = glm::vec4 { 
		(float)255 / 255,
		(float)255 / 255,
		(float)255 / 255, 1.0f 
	};

	glm::vec4 skyColor = glm::vec4 {
		(float)90 / 255,
		(float)86 / 255,
		(float)150 / 255, 1.0f
	};

	// ===

	glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	// ===

	Shader materialShader("default.vert", "material.frag");
	Shader unlitShader("default.vert", "unlit_color.frag");
	Shader shadowMapShader("shadow_map.vert", "shadow_map.frag");

	Shader chunkShader("chunk.vert", "chunk.frag");
	Shader chunkShadowMap("chunk_shadow.vert", "shadow_map.frag");

	// ===

	std::vector <Texture> woodMaterial {
		Texture("planks.png", "diffuse", 0),
		Texture("planks_specular.png", "specular", 1)
	};

	std::vector <Texture> atlasMaterial {
		Texture("atlas.png", "diffuse", 0)
		//Texture("texture_atlas_specular.png", "specular", 1)
	};

	// ===

	std::vector<Vertex> cubeVerts{};
	std::vector<GLuint> cubeTris{};
	glm::mat4 cubeMatrix;

	getCubeMesh(cubeVerts, cubeTris, cubeMatrix);
	Mesh cubeMesh{ cubeVerts, cubeTris, cubeMatrix };

	// ===

	const auto atlas = generateAtlas();
	const auto chunkSize = 16;
	const auto renderRadius = 5;
	World world{ chunkSize, renderRadius };

	// ===

	Object sun{};
	sun.position(glm::vec3{ 0.5f, 0.4f, 0.5f } * 20.0f);
	sun.color(sunColor);

	Object centerLine{};
	centerLine.scale(glm::vec3{ 0.2f, 100.0f, 0.02f });
	centerLine.color(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	
	Object center{};
	center.scale(glm::vec3{ 0.5f });
	center.color(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	Object forward{};
	forward.position(glm::vec3{ 0.0f, 0.0f, 3.0f });
	forward.color(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	Object right{};
	right.position(glm::vec3{ 3.0f, 0.0f, 0.0f });
	right.color(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	Object up{};
	up.position(glm::vec3{ 0.0f, 3.0f, 0.0f });
	up.color(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	Object ground{};
	ground.position(glm::vec3{ 0.0f, -3.0f, 0.0f });
	ground.scale(glm::vec3{ 100.0f, 1.0f, 100.0f });

	// ===

	glEnable(GL_DEPTH_TEST);

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// METERS PER SECOND.
	const auto standardSpeed = 15.0f;
	const auto sensitivity = 0.1f;
	Camera camera{ window, width, height, standardSpeed, sensitivity };

	float previousTime = 0.0;
	float currentTime = 0.0;
	float timer = 0.0;

	// DISABLE VSYNC.
	glfwSwapInterval(0);
	ShadowMapFBO shadowMap{ 2048, 2048, 25.0f };

	while (!glfwWindowShouldClose(window)) {
		currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - previousTime;

		// MAYBE USE THREAD.SLEEP FOR THIS?
		// SINCE WE WANT TO AVOID BUSY WAITING ...
		// https://discourse.glfw.org/t/frame-limiting/70/4
		// MAYBE THAT'S FINE IDK
		if (deltaTime < minDeltaForFrame)
			continue;

		previousTime = currentTime;

		std::string fps = std::to_string(1.0f / deltaTime);
		std::string ms = std::to_string(deltaTime * 1000);
		std::string newTitle = "fps: " + fps + " | ms: " + ms;
		glfwSetWindowTitle(window, newTitle.c_str());

		// ===

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			world.tick(camera.position);
			world.flush(atlas);
			// std::cout << "lgihtwork" << std::endl;
		}

		camera.hasFocus = hasFocus;
		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(105.0f, 0.01f, 100.0f);


		shadowMap.bind(camera, sun, chunkShadowMap);

		//ground.drawAsUnlitColor(cubeMesh, shadowMapShader, camera);
		world.drawShadows(chunkShadowMap, camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// ===
		
		//ground.drawWithMaterial(cubeMesh, woodMaterial, materialShader, camera, sunColor, sun.pos, skyColor);
		// sun.drawAsUnlitColor(cubeMesh, unlitShader, camera);

		// centerLine.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		// forward.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		// center.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		// right.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		// up.drawAsUnlitColor(cubeMesh, unlitShader, camera);

		shadowMap.sendToShader(chunkShader);
		world.draw(atlasMaterial, chunkShader, camera, sun.col, sun.pos, skyColor);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	materialShader.free();
	unlitShader.free();

	world.free();

	freeMaterial(woodMaterial);
	freeMaterial(atlasMaterial);

	cubeMesh.free();
	shadowMap.free();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}