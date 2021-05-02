#include "Sprite.h" 

class Character {

    private:

        Sprite* sprite;

        int hp            = 100;
        int attack_damage = 10;
        float x_position  = 0;
        float y_position  = 0;

    public: 


        Character(Sprite* i_sprite, float x, float y);
        
        void attack(Character enemy);

        void setSprite(Sprite* i_sprite);

        void receive_damage(int damage);

        int getHP();

        void walk_right();

        void walk_left();

};