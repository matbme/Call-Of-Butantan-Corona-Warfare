# Call-Of-Butantan-Corona-Warfare

## Compiling

We must specify all `.cpp` files to be compiled, as well as some libs to go with it.

```bash
g++ src/Source.cpp src/glad.c src/SceneManager.cpp src/Sprite.cpp src/stb_image.cpp src/Character.cpp -g -o app -Iinclude -lglfw -ldl
``` 