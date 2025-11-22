// https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

#include "Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // THIS SHOULD BE 4, BUT THIS WORKS.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// HERE YOU CAN SET FULLSCREEN OR NOT.
	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraft Clone", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	// ===

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.activate();
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	framebufferProgram.activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.id, "screenTexture"), 0);

	// ===

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// ===

	Model model{ "models/map/scene.gltf" };

	// Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	double fpsCap = 300.0;
	double minDeltaTimeForFrame = 1.0 / fpsCap;
	const float tickInterval = 0.01f;
	float timer = 0.0f;

	double prevTime = 0.0;
	double crntTime = 0.0;
	double deltaTime;
	unsigned int counter = 0;

	// Create Frame Buffer Object
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create Framebuffer Texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// DISABLE VSYNC
	glfwSwapInterval(0);

	while (!glfwWindowShouldClose(window)) {
		crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;

		if (deltaTime < minDeltaTimeForFrame)
			continue;

		++counter;
		std::string fps = std::to_string((1.0 / deltaTime) * counter);
		std::string ms = std::to_string((deltaTime / counter) * 1000);
		std::string newTitle = "fps: " + fps + " | ms: " + ms;
		glfwSetWindowTitle(window, newTitle.c_str());
		prevTime = crntTime;
		counter = 0;

		// Bind the custom framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable depth testing since it's disabled when drawing the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);

		timer += (float)deltaTime;
		while (timer > tickInterval) {
			timer -= tickInterval;
			camera.moveCamera(window, tickInterval);
			camera.updateMatrix(45.0f, 0.1f, 100.0f);
		}

		model.draw(shaderProgram, camera);

		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		framebufferProgram.activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// https://youtu.be/QQ3jr-9Rc1o?si=2S_BGxm-etPpO9xY&t=220

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	model.free();
	shaderProgram.free();
	glDeleteFramebuffers(1, &fbo);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}