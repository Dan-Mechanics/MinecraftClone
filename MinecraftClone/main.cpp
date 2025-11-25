// https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

#include "Mesh.h"
#include "Cube.h"

const unsigned int width = 1920;
const unsigned int height = 1080;
const double fpsCap = 300.0;
const double minDtForFrame = 1.0 / fpsCap;
const double tickInterval = 0.02;

// Vertices coordinates
Vertex vertices[] = { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] = {
	0, 2, 1,
	0, 3, 2
};


int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // THIS SHOULD BE 4, BUT THIS WORKS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFWwindow* window = glfwCreateWindow(width, height, "My Title", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft Clone", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	//glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, 144);

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// ===

	Texture textures[]{
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};


	// Generates Shader object using shaders default.vert and default.frag
	Shader defaultShader("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);


	// Shader for light cube
	Shader lightShader("default.vert", "light.frag");
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Cube lightCube{ glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{0.0f} , glm::vec3{0.25f} };

	Cube bouncingCube{ glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{1.0f} };
	bouncingCube.setColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	glm::vec3 objectPos{};
	glm::mat4 objectModel = glm::mat4{ 1.0f };
	objectModel = glm::translate(objectModel, objectPos);

	/*lightShader.activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(lightShader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	defaultShader.activate();
	glUniformMatrix4fv(glGetUniformLocation(defaultShader.id, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(defaultShader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(defaultShader.id, "lightPos"), lightCube.pos.x, lightCube.pos.y, lightCube.pos.z);*/

	// ===

	glEnable(GL_DEPTH_TEST);

	// https://learnopengl.com/Advanced-OpenGL/Face-culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// METERS PER SECOND.
	const auto standardSpeed = 1.0f;
	const auto sensitivity = 0.1125f;
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

		/*const auto r = 12 / 255.0f;
		const auto g = 12 / 255.0f;
		const auto b = 40 / 255.0f;*/
		glClearColor(0.5, 0.5, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
			//camera.moveCamera(window, tickInterval);

			lightColor.r += (float)tickInterval * (increase ? 1.0f : -1.0f);
			if (lightColor.r > 1.0f) {
				lightColor.r = 1.0f;
				increase = !increase;
			}

			if (lightColor.r < 0.0f) {
				lightColor.r = 0.0f;
				increase = !increase;
			}

			lightColor.g = lightColor.r;
			//lightCube.pos = -camera.position;
			//lightCube.pos.y = 1.0f;
			bouncingCube.rot.y += 1;
			lightCube.setColor(lightColor);
		}

		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(103.0f, 0.01f, 100.0f);

		lightCube.draw(lightShader, camera);

		floor.draw(defaultShader, camera, objectModel, glm::vec3{ 0.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, lightCube.pos, lightColor);

		bouncingCube.draw(lightShader, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			break;
	}

	floor.free();
	lightCube.free();
	defaultShader.free();
	lightShader.free();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}