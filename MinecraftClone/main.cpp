#include "Mesh.h"
#include "Object.h"
#include "utils.h"
#include "world_mesh.h"
#include <unordered_map>
#include "BlockType.h"

const unsigned int width = 1920;
const unsigned int height = 1080;
const float fpsCap = 300.0f;
const float minDtForFrame = 1.0f / fpsCap;
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

	glm::vec4 skyColor = glm::vec4(
		(float)100 / 255,
		(float)145 / 255,
		(float)190 / 255,
		1.0f);

	glm::vec4 sunColor = glm::vec4((float)255 / 255, (float)255 / 255, (float)255 / 255, 1.0f);

	// ===

	Shader materialShader("default.vert", "material.frag");
	Shader unlitShader("default.vert", "unlit_color.frag");
	Shader shadowMapShader("shadow_map.vert", "shadow_map.frag");

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
	const auto world = generateDemoWorld();

	std::vector<Vertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};
	glm::mat4 chunkMatrix;

	/*generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, world.at({ 0, 0, 0 }), world);
	Mesh chunkMesh1{ chunkVerts, chunkTris, chunkMatrix };

	generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, world.at({ 0, 0, 1 }), world);
	Mesh chunkMesh2{ chunkVerts, chunkTris, chunkMatrix };*/

	std::vector<Mesh> chunks{};
	auto it = world.begin();
	while (it != world.end()) {
		generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, it->second, world);
		chunks.emplace_back(chunkVerts, chunkTris, chunkMatrix);
		++it;
	}

	// ===

	Object sun{ glm::vec3{ 0.5f, 0.5f, 0.5f } * 20.0f, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };
	sun.setColor(sunColor);

	Object centerLine{ glm::vec3{ 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.2f, 100.0f, 0.02f } };
	centerLine.setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	
	Object center{ glm::vec3{ 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 0.5f } };
	center.setColor(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	Object forward{ glm::vec3{ 0.0f, 0.0f, 3.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };
	forward.setColor(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	Object right{ glm::vec3{ 3.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };
	right.setColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	Object up{ glm::vec3{ 0.0f, 3.0f, 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };
	up.setColor(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	Object ground{ glm::vec3{0.0f, -3.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{100.0f, 1.0f, 100.0f} };
	Object chunk{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };
	//Object chunk2{ glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };

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

	float previousTime = 0.0;
	float currentTime = 0.0;
	float timer = 0.0;

	// DISABLE VSYNC.
	glfwSwapInterval(0);



	// Framebuffer for Shadow Map
	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);

	// Texture for Shadow Map FBO
	unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "shadow framebuffer broken!!" << std::endl;
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	while (!glfwWindowShouldClose(window)) {
		currentTime = (float)glfwGetTime();
		float deltaTime = currentTime - previousTime;

		// MAYBE USE THREAD.SLEEP FOR THIS?
		// SINCE WE WANT TO AVOID BUSY WAITING ...
		// https://discourse.glfw.org/t/frame-limiting/70/4
		// MAYBE THAT'S FINE IDK
		if (deltaTime < minDtForFrame)
			continue;

		previousTime = currentTime;

		std::string fps = std::to_string(1.0f / deltaTime);
		std::string ms = std::to_string(deltaTime * 1000);
		std::string newTitle = "fps: " + fps + " | ms: " + ms;
		glfwSetWindowTitle(window, newTitle.c_str());

		// ===

		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// DRAW SCENE FOR SHADOW MAP !!

		//ground.drawAsUnlitColor(cubeMesh, shadowMapShader, camera);
		//chunk.drawAsUnlitColor(chunkMesh, shadowMapShader, camera);
		
		// HERE YOU CAN CHANGE THE RESOLUTION
		// OF THE SHADOWS.
		const auto dist = 25.0f;
		glm::mat4 orthgonalProjection = glm::ortho(-dist, dist, -dist, dist, -dist, dist);
		glm::mat4 lightView = glm::lookAt(glm::normalize(sun.pos), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// I THINK THIS IS CORRECT, I DO NOT KNOW.
		glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, -camera.position);
		glm::mat4 lightProjection = orthgonalProjection * lightView * translation;

		shadowMapShader.activate();
		glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.id, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

		//ground.drawAsUnlitColor(cubeMesh, shadowMapShader, camera);
		auto it2 = chunks.begin();
		while (it2 != chunks.end()) {
			chunk.drawAsUnlitColor(*it2, shadowMapShader, camera);
			++it2;
		}

		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Switch back to the default viewport
		glViewport(0, 0, width, height);
		// Bind the custom framebuffer
		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Switch back to the default viewport
		glViewport(0, 0, width, height);
		// Specify the color of the background
		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth testing since it's disabled when drawing the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			//std::cout << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
			//chunk.rotate(glm::vec3{ 0.0f, 90.0f, 0.0f }, deltaTime);
		}

		camera.hasFocus = hasFocus;
		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(103.0f, 0.01f, 100.0f);

		materialShader.activate();
		glUniformMatrix4fv(glGetUniformLocation(materialShader.id, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glUniform1i(glGetUniformLocation(materialShader.id, "shadowMap"), 2);

		//ground.drawWithMaterial(cubeMesh, woodMaterial, materialShader, camera, sunColor, sun.pos, skyColor);
		//sun.drawAsUnlitColor(cubeMesh, unlitShader, camera);

		centerLine.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		center.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		forward.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		right.drawAsUnlitColor(cubeMesh, unlitShader, camera);
		up.drawAsUnlitColor(cubeMesh, unlitShader, camera);

		/*chunk.drawWithMaterial(chunkMesh1, atlasMaterial, materialShader, camera, sunColor, sun.pos, skyColor);
		chunk2.drawWithMaterial(chunkMesh2, atlasMaterial, materialShader, camera, sunColor, sun.pos, skyColor);*/

		it2 = chunks.begin();
		while (it2 != chunks.end()) {
			chunk.drawWithMaterial(*it2, atlasMaterial, materialShader, camera, sunColor, sun.pos, skyColor);
			++it2;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	materialShader.free();
	unlitShader.free();

	freeMaterial(woodMaterial);
	freeMaterial(atlasMaterial);

	glDeleteFramebuffers(1, &shadowMapFBO);

	cubeMesh.free();
	//chunkMesh1.free();

	// make sure to delete the shadow map buffer

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}