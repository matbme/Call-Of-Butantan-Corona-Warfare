#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_mixer/SDL_mixer.h>  
#else
#include <SDL2/SDL_mixer.h>  
#endif

#include <string>

using namespace std;


#define BITS 8

class AudioManager {

    public:

        static AudioManager* get_instance();

        void init();
        void play_audio(const char* path);
        void cleanup();
        
    private:

     Mix_Chunk *wave = NULL;
 
};
