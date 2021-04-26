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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupGeometry();
void error_callback(int code, const char* description);
void updateWalk(int x, int y, int action);

const GLuint WIDTH = 1280, HEIGHT = 720;

int walkX = 0;
int walkY = 0;

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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CoB: CW",
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
	Shader shader("src/shaders/transformations.vs", "src/shaders/transformations.frag");
	
	// Test Geometry
	GLuint VAO = setupGeometry();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shader.ID, "contourColor");
	assert(colorLoc > -1);
	glUseProgram(shader.ID);
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f); //para cor de contorno

	// Para enviar o identificador de modo de desenho
	GLint rasterCodeLoc = glGetUniformLocation(shader.ID, "rasterCode");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // background color
		glClear(GL_COLOR_BUFFER_BIT);

		// Relocate Viewport based on window size
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glm::mat4 model = glm::mat4(1);
		glm::mat4 ortho = glm::mat4(1);;

		// Camera
		ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

		// Transformation 1
		model = glm::translate(model, glm::vec3(300.0f, 100.0f, 0.0f)); 
		model = glm::translate(model, glm::vec3((float) walkX, (float) walkY, 0.0f)); 
		model = glm::translate(model, glm::vec3(-300.0f, -100.0f, 0.0f)); 

		// Apply transform to shader
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		GLint projLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

		glUniform1i(rasterCodeLoc, 0);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key) {
		case 87:				// W
			updateWalk(0, -5, action);
			break;
		case 65:				// A
			updateWalk(-5, 0, action);
			break;
		case 83:				// S
			updateWalk(0, 5, action);
			break;
		case 68:				// D
			updateWalk(5, 0, action);
			break;
		default:
			break;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void updateWalk(int x, int y, int action) {
	if (action == GLFW_PRESS) {
		walkX += x;
		walkY += y;
	}
}

int setupGeometry()
{
	//		Coordenada 		   		  Cor
	GLfloat vertices[] = {
		300.0f, 150.0f, 0.0f,	 1.0, 1.0, 0.0, //Superior
	    200.0f, 350.0f, 0.0f,	 1.0, 0.0, 0.0, //Inferior esquerdo
		400.0f, 350.0f, 0.0f,	 1.0, 0.0, 0.0, //Inferior direito
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//Atributo Coordenada
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
		((GLvoid*)(3 * sizeof(GLfloat))));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0); 

	return VAO;
}

void error_callback(int code, const char* description)
{
	std::cout << "GLFW Error:" << description << std::endl; 
}
