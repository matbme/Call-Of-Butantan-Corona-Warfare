CC		= g++
CFLAGS	= -g -Iinclude
LIBS	= -lglfw -ldl -lSDL2main -lSDL2 -lSDL2_mixer

SOURCE	= $(wildcard src/*.cpp src/*.c)

app: $(SOURCE)
	$(CC) $(CFLAGS) $? $(LIBS) -o $@
	
clean:
	-rm -rfv app app.dSYM
