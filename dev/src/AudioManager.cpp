#include "AudioManager.h"
#include <math.h>
#include <stdio.h>
 

void AudioManager::init(){ 
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) ;
}

void AudioManager::play_audio(const char* path){
    wave = Mix_LoadWAV(path);
    Mix_PlayChannel(-1, wave, 0);
}

void AudioManager::cleanup(){ 
    	Mix_FreeChunk(wave);
	Mix_CloseAudio();

}
