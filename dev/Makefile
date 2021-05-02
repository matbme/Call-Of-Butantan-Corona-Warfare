CC		= g++
CFLAGS	= -g -Iinclude
LIBS	= -lglfw -ldl

SOURCE	= $(wildcard src/*.cpp src/*.c)

app: $(SOURCE)
	$(CC) $(CFLAGS) $? $(LIBS) -o $@
	
clean:
	-rm -rfv app app.dSYM
