#include "Sprite.h" 
#include <string>

using namespace std;

class Character {

    private:
        int hp            = 100;
        int attack_damage = 10;
		bool attack_lock  = false;

    public: 
		bool is_jumping = false;
		int jump_ticks = 0;

        Sprite* sprite;
        float x_position  = 0;
        float y_position  = 0;

        Character(Sprite* i_sprite, float x, float y);

	    unsigned int loadTexture(string filename); 

        
        void attack(Character enemy);

        void setSprite(Sprite* i_sprite);

        void receive_damage(int damage);

        int getHP();

        void walk_right();

        void walk_left();

		void jump();

		void update_jump_pos();

		bool checkWorldCollision(float worldBorder);

		void toggle_attack_lock();

		bool attack_locked();
};
