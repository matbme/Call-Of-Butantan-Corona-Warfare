#include "Character.h" 


Character::Character(Sprite* i_sprite, float x, float y){
    this->sprite     = i_sprite;
    this->x_position = x;
    this->y_position = y;
}

void Character::setSprite(Sprite* i_sprite){
    this->sprite = i_sprite;
}

void Character::attack(Character enemy){
    enemy.receive_damage(this->attack_damage);
}

void Character::receive_damage(int damage){
    this->hp -= damage; 
}

int Character::getHP(){
    return this->hp;
}

void Character::walk_right(){
	this->x_position += 10.0f;
    this->sprite->setPosition(glm::vec3(this->x_position, this->y_position, 0.0));
}

void Character::walk_left(){
	this->x_position -= 10.0f;
    this->sprite->setPosition(glm::vec3(this->x_position, this->y_position, 0.0));
}
 
unsigned int Character::loadTexture(string filename)
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
