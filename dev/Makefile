CC		= g++
CFLAGS	= -g -Iinclude
LIBS	= -lglfw -ldl -lSDL2main -lSDL2 -lSDL2_mixer

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
