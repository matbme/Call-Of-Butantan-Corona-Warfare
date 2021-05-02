#include <iostream>
#include <string>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
* Defines an actor on screen
*/
class Actor {
	public:
		float height;				// Actor size in Y
		float width;				// Actor size in X

		float posX;					// Actor initial position in X
		float posY;					// Actor initial position in Y

		float offsetX = 0;			// Actor offset in X
		float offsetY = 0;			// Actor offset in Y

		Actor() {					// Default actor
			height = 10;
			width = 5;
		}

		Actor(float x, float y) {	// Custom actor
			height = y;
			width = x;
		}

		int setupActor(float posX, float posY) {
			this->posX = posX;
			this->posY = posY;

			GLfloat vertices[] = {
				posX		, posY		   , 0.0f,
				posX + width, posY		   , 0.0f,
				posX		, posY + height, 0.0f,
				posX + width, posY + height, 0.0f
			};

			GLuint indices[] = {
				0, 1, 2,
				2, 3, 1
			};

			GLuint VBO, VAO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
					     GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 
	                     GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
								  (void*)0);
			glEnableVertexAttribArray(0);

			return VAO;
		}

		void move(float moveX, float moveY) {
			this->offsetX += moveX;
			this->offsetY += moveY;
		}


		glm::mat4 movementTransform() {
			glm::mat4 model = glm::mat4(1);

			model = glm::translate(model, glm::vec3(posX + (width/2),
													posY + (height/2),
													0.0f)); 

			model = glm::translate(model, glm::vec3(offsetX, offsetY, 0.0f)); 

			model = glm::translate(model, glm::vec3(-(posX + (width/2)),
													-(posY + (height/2)),
													0.0f)); 
	
			return model;
		}
};
