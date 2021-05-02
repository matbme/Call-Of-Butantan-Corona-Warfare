#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "actor.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void error_callback(int code, const char* description);

const GLuint WIDTH = 1280, HEIGHT = 720;

// Reference to player actor for callback
Actor *playerRef = NULL;

int main()
{
	// Libs initialization
	glfwInit();

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, 
										  "Call Of Butantan: Corona Warfare",
										  nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION); 
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Set viewport for window size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	cout << width << "x" << height << endl;
	glViewport(0, 0, width, height);

	// Shader init
	Shader shader("src/shaders/transformations.vs", 
				  "src/shaders/transformations.frag");
	
	// Player creation
	Actor player;
	playerRef = &player;
	GLuint VAO = player.setupActor(100.0, 50.0);

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "contourColor");
	assert(colorLoc > -1);
	glUseProgram(shader.ID);
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f); //para cor de contorno

	// Pra enviar o identificador de modo de desenho
	GLint rasterCodeLoc = glGetUniformLocation(shader.ID, "rasterCode");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // background color
		glClear(GL_COLOR_BUFFER_BIT);

		// Relocate Viewport based on window size
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Camera
		glm::mat4 ortho = glm::mat4(1);;
		ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

		// Transformation 1
		glm::mat4 model = player.movementTransform();

		// Apply transform to shader
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		GLint projLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

		glUniform1i(rasterCodeLoc, 0);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	assert(playerRef != NULL);

	switch (key) {
		case 87:				// W
			if (action != GLFW_RELEASE)
				playerRef->move(0.0, -5.0);
			break;
		case 65:				// A
			if (action != GLFW_RELEASE)
				playerRef->move(-5.0, 0.0);
			break;
		case 83:				// S
			if (action != GLFW_RELEASE)
				playerRef->move(0.0, 5.0);
			break;
		case 68:				// D
			if (action != GLFW_RELEASE)
				playerRef->move(5.0, 0.0);
			break;
		default:
			break;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void error_callback(int code, const char* description)
{
	std::cout << "GLFW Error:" << description << std::endl; 
}
