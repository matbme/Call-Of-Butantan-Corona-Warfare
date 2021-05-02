#include "SceneManager.h"

static bool keys[1024];
static bool resized;
static GLuint width, height;

int hold_space_times = 0;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;

	initializeGraphics();
}

void SceneManager::initializeGraphics()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, "Final Battle", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	addShader("shaders/transformations.vs", "shaders/transformations.frag");

	setupScene();

	resized = true;
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}

void SceneManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow *window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	glViewport(0, 0, width, height);
}

void SceneManager::update()
{
	unsigned int textID;

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_RIGHT])
		if (!characters[0]->checkWorldCollision(width))
			characters[0]->walk_right();

	if (keys[GLFW_KEY_LEFT])
		if (!characters[0]->checkWorldCollision(width))
			characters[0]->walk_left();

	if (keys[GLFW_KEY_D])
		if (!characters[1]->checkWorldCollision(width))
			characters[1]->walk_right();

	if (keys[GLFW_KEY_A])
		if (!characters[1]->checkWorldCollision(width))
			characters[1]->walk_left();

	if (keys[GLFW_KEY_W])
		characters[1]->jump();

	if (keys[GLFW_KEY_SPACE]){
		if(hold_space_times<5){
			hold_space_times++;
		}else{
 			keys[GLFW_KEY_SPACE] = false;

			hold_space_times=0;

			characters[0]->receive_damage(10);
			int hp = characters[0]->getHP();

			if(hp >= 10){
				textID = loadTexture("textures/hp/hp-0" + std::to_string(hp/10) + ".png");
				objects[5]->setTexture(textID);
			}
		}
	}

	if (keys[GLFW_KEY_ENTER]) {
		if (hold_space_times < 5) hold_space_times++;
		else {
			keys[GLFW_KEY_RIGHT_CONTROL] = false;
			hold_space_times = 0;

			characters[1]->receive_damage(10);
			int hp = characters[1]->getHP();

			if (hp >= 10) {
				textID = loadTexture("textures/hp/hp-0" + std::to_string(hp/10) + ".png");
				objects[3]->setTexture(textID);
			}

		}
	}

	// Jump updating
	for (Character *chr : characters) {
		if (chr->is_jumping) {
			chr->jump_ticks++;
			chr->update_jump_pos();
		}
	}
}

void SceneManager::render()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (resized)
	{
		setupCamera2D();
		resized = false;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}
}

void SceneManager::run()
{
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		update();
		render();
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	glfwTerminate();
}

void SceneManager::setupScene()
{
	create_background();
	create_characters();
	create_hp_bar();

	ortho2D[0] = 0.0f;	  //xMin
	ortho2D[1] = 1600.0f; //xMax
	ortho2D[2] = 0.0f;	  //yMin
	ortho2D[3] = 1200.0f; //yMax

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::create_background()
{

	Sprite *sprite = new Sprite;
	unsigned int texID = loadTexture("textures/background.jpg");

	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(800.0f, 600.0f, 0.0));
	sprite->setDimension(glm::vec3(1600.0f, 1200.0f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);
}

void SceneManager::create_characters()
{

	Sprite *sprite;
	unsigned int texID;
	Character *character;

	float char1_x = 1000.0f;
	float char2_x = 600.0f;

	sprite = new Sprite;
	texID = loadTexture("textures/naruto.png");
	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(char1_x, 260.0, 0.0));
	sprite->setDimension(glm::vec3(185.5f, 200.5f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	character = new Character(sprite, char1_x, 260.0f);
	characters.push_back(character);

	sprite = new Sprite;
	texID = loadTexture("textures/sasuke.png");
	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(char2_x, 260.0, 0.0));
	sprite->setDimension(glm::vec3(280.0f, 280.0f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	character = new Character(sprite, char2_x, 260.0f);
	characters.push_back(character);
}

void SceneManager::create_hp_bar()
{

	unsigned int texID;
	Sprite *sprite;

	sprite = new Sprite;
	texID = loadTexture("textures/hp/hp-10.png");
	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(410.0f, 1050.0f, 0.0));
	sprite->setDimension(glm::vec3(435.0, 72.5f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	sprite = new Sprite;
	texID = loadTexture("textures/ze-gotinha-mask.png");
	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(700.0f, 1100.0f, 0.0));
	sprite->setDimension(glm::vec3(150.0, 150.0f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	sprite = new Sprite;
	texID = loadTexture("textures/hp/hp-10.png");
	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(1190.0f, 1050.0f, 0.0));
	sprite->setDimension(glm::vec3(435.0, 72.5f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

	sprite = new Sprite;
	texID = loadTexture("textures/mito.png");
	sprite->setTexture(texID);
	sprite->setPosition(glm::vec3(900.0f, 1100.0f, 0.0));
	sprite->setDimension(glm::vec3(150.0, 150.0f, 1.0f));
	sprite->setShader(shader);
	objects.push_back(sprite);

}

void SceneManager::setupCamera2D()
{
	float zNear = -1.0, zFar = 1.0;

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");

	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	return texture;
}
