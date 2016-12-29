CC = g++
CFLAGS = -m64 -c -Wall -Wextra -I/mingw64/include -Iinclude -std=c++14 
	
ifeq ($(OS),Windows_NT)
	LDFLAGS = -L/mingw64/lib -lopengl32 -lglew32 -lglfw3 -lassimp -lvulkan-1
else
	LDFLAGS = -lGL -lGLEW -lglfw -lassimp -lvulkan -lSOIL
endif
SOURCES := $(wildcard  src/*.cpp src/game/*.cpp src/core/*.cpp src/light/*.cpp src/render/*.cpp src/render/opengl/*.cpp )
EXECUTABLE=hello.exe

ALL_SOURCES := src/main.cpp $(SOURCES) 
ALL_OBJECTS := $(SOURCES:.cpp=.o)


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(ALL_OBJECTS)
	$(CC) $(ALL_OBJECTS)  $(LDFLAGS) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm src/*.o src/*/*.o src/*/*/*.o *.exe