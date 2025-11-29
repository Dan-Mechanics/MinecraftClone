// https://www.youtube.com/playlist?list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

#include "Mesh.h"
#include "Cube.h"

const unsigned int width = 1920;
const unsigned int height = 1080;
const double fpsCap = 300.0;
const double minDtForFrame = 1.0 / fpsCap;
const double tickInterval = 0.02;

bool hasFocus = true;
//Vertex vertices[] = { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
//	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
//	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
//};

//Vertex vertices[] = { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
//	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
//	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
//	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
//};

// Vertices coordinates
//GLfloat vertices[] =
//{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
//	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, 
//	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, 
//	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f,		
//	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f,	
//
//	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f,	
//	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f,	
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, 	
//
//	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, 
//	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, 
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, 	
//
//	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, 	
//	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, 	
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f,	
//
//	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, 	
//	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, 
//	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  	
//};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

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

	glm::vec4 worldColor = glm::vec4((float)110/255, (float)164/255, (float)230 / 255, 1.0f);

	Texture textures[]{
		Texture("planks.png", "diffuse", 0),
		Texture("_planksSpec.png", "specular", 1)
	};

	std::vector<Vertex> pyramidVerts{};
	// (const glm::vec3 & position, const glm::vec3 & normal, const glm::vec3 & color, const glm::vec2 & texUv)
	pyramidVerts.emplace_back(glm::vec3{ -0.5f, 0.0f, 0.5f }, glm::vec3{  0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 5.0f });
	pyramidVerts.emplace_back(glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{  0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 5.0f });
	pyramidVerts.emplace_back(glm::vec3{ 0.5f, 0.0f,  0.5f }, glm::vec3{  0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	
	pyramidVerts.emplace_back(glm::vec3{ -0.5f, 0.0f,  0.5f }, glm::vec3{  -0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{  -0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{  0.0f, 0.8f,  0.0f }, glm::vec3{  -0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 2.5f, 5.0f });

	pyramidVerts.emplace_back(glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.5f, -0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{  0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.5f, -0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{  0.0f, 0.8f,  0.0f}, glm::vec3{ 0.0f, 0.5f, -0.8f}, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{   2.5f, 5.0f });

	pyramidVerts.emplace_back(glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ 0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{ 0.5f, 0.0f,  0.5f }, glm::vec3{ 0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{ 0.0f, 0.8f,  0.0f}, glm::vec3{  0.8f, 0.5f,  0.0f }, glm::vec3{ 0.92f, 0.86f, 0.76f }, glm::vec2{ 2.5f, 5.0f });

	pyramidVerts.emplace_back(glm::vec3{  0.5f, 0.0f,  0.5f }, glm::vec3{ 0.0f, 0.5f,  0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{ -0.5f, 0.0f,  0.5f }, glm::vec3{ 0.0f, 0.5f,  0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	pyramidVerts.emplace_back(glm::vec3{  0.0f, 0.8f,  0.0f}, glm::vec3{  0.0f, 0.5f,  0.8f }, glm::vec3{ 0.92f, 0.86f, 0.76f }, glm::vec2{ 2.5f, 5.0f });

	std::vector<Vertex> cubeVerts{};
	cubeVerts.emplace_back(glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{  1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.5f, 0.5f, -0.5f }, glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{  1.0f, 1.0f });
	cubeVerts.emplace_back(glm::vec3{ -0.5f, 0.5f, -0.5f }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 1.0f });
	
	cubeVerts.emplace_back(glm::vec3{ -0.5f, -0.5f, 0.5f }, glm::vec3{ -1.0f, 0.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{  0.5f, -0.5f, 0.5f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{  1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{  0.5f,-0.5f, 0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{  1.0f, 1.0f });
	cubeVerts.emplace_back(glm::vec3{ -0.5f, 0.5f, 0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{  0.0f, 1.0f });

	//Vertex vertices[16];
	//std::copy(pyramidVerts.begin(), pyramidVerts.end(), vertices);

	// Generates Shader object using shaders default.vert and default.frag
	Shader defaultShader("default.vert", "worldlight.frag");
	// Store mesh data in vectors for the mesh
	//std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(pyramidVerts, ind, tex);


	// Shader for light cube
	Shader cubeShader("default.vert", "lit_color.frag");
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Cube lightCube{ cubeVerts, glm::vec3{0.5f, 0.5f, 0.5f}, glm::vec3{0.0f} , glm::vec3{0.25f} };


	Cube redCube{ cubeVerts, glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{0.5f} };
	redCube.setColor(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	Cube blueCube{ cubeVerts, glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{0.51f} };
	blueCube.setColor(glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	Cube ground{ cubeVerts, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec3{0.0f}, glm::vec3{100.0f, 1.0f, 100.0f} };
	ground.setColor(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.0f });

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

		glClearColor(worldColor.r, worldColor.g, worldColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timer += deltaTime;
		while (timer >= tickInterval) {
			timer -= tickInterval;
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
			lightCube.setColor(lightColor);

			redCube.rot.y += 1.0f;
			/*redCube.pos.y += tickInterval;
			if (redCube.pos.y > 3.0f)
				redCube.pos.y = 0.0f;*/

			blueCube.rot.y -= 1.0f;
			/*blueCube.pos.y -= tickInterval;
			if (blueCube.pos.y < -3.0f)
				blueCube.pos.y = 0.0f;*/
		}

		camera.hasFocus = hasFocus;
		camera.moveCamera(window, deltaTime);
		camera.rotateCamera(window);
		camera.updateMatrix(103.0f, 0.01f, 100.0f);

		lightCube.draw(cubeShader, camera, lightColor, lightCube.pos, worldColor);

		floor.draw(defaultShader, camera, objectModel, glm::vec3{ 0.0f }, glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f },
			glm::vec3{ 1.0f }, lightCube.pos, lightColor, worldColor);

		ground.draw(cubeShader, camera, lightColor, lightCube.pos, worldColor);
		redCube.draw(cubeShader, camera, lightColor, lightCube.pos, worldColor);
		blueCube.draw(cubeShader, camera, lightColor, lightCube.pos, worldColor);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			break;
	}

	floor.free();
	lightCube.free();
	defaultShader.free();
	cubeShader.free();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}