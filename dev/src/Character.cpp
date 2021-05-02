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
	x_position += 10.0f;
    this->sprite->setPosition(glm::vec3(x_position, y_position, 0.0));
}

void Character::walk_left(){
	x_position -= 10.0f;
    this->sprite->setPosition(glm::vec3(x_position, y_position, 0.0));
}
 