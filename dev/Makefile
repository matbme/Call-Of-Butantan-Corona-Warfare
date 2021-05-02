ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname)
endif

CC		= g++
CFLAGS	= -g -Iinclude
LIBS	= -lglfw -ldl

ifeq ($(detected_OS),Darwin)
	CFLAGS	+= -F/Library/Frameworks
	LIBS	+= -framework SDL2 -I/Library/Frameworks/SDL2.framework/Headers -framework SDL2_mixer -I/Library/Frameworks/SDL2_mixer.framework/Headers
endif
ifeq ($(detected_OS),Linux)
	LIBS	+= -lSDL2main -lSDL2 -lSDL2_mixer
endif

ODIR	= src/obj
SDIR	= src

SOURCES_C := $(wildcard $(SDIR)/*.c) 
SOURCES_CPP += $(wildcard $(SDIR)/*.cpp) 

OBJECTS_C := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES_C))
OBJECTS_CPP += $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES_CPP))

OBJECTS := $(OBJECTS_C) $(OBJECTS_CPP)
SOURCES := $(SOURCES_C) $(SOURCES_CPP)

app: $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY: clean

clean:
	-rm -rfv app app.dSYM
