TARGET := assignment2.exe
CLEAN := del .\obj\*.o .\obj\models\*.o $(TARGET)
CXX := g++

CFLAGS_NIX := -c -Wall -Wextra -std=c++11 -I./inc
CFLAGS_WIN := -c -Wall -Wextra -std=c++11 -I".\inc" -I"C:\MinGW\freeglut\include"

LFLAGS_NIX := -lm -lGL -lGLU -lglut
LFLAGS_WIN := -lm -L"C:\MinGW\freeglut\lib" -lfreeglut -lopengl32 -lglu32 -Wl,--subsystem,windows

MODELS_SRC = $(wildcard ./src/models/*.cpp)
MODELS_OBJ = $(patsubst ./src/models/%.cpp, ./obj/models/%.o, $(MODELS_SRC))

MAKE_OBJ = $(CXX) $(CFLAGS_NIX) $< -o $@
MAKE_EXE = $(CXX) $(LFLAGS_NIX) $^ -o $@

$(TARGET): obj/main.o obj/scene.o obj/vec3.o $(MODELS_OBJ)
	$(MAKE_EXE)

test.exe: obj/test.o obj/scene.o obj/vec3.o $(MODELS_OBJ)
	$(CXX) -static -lm $^ -o $@

models: $(MODELS_OBJ)

obj/models/%.o: src/models/%.cpp inc/vec3.hpp inc/models.hpp
	$(MAKE_OBJ)

obj/main.o: src/main.cpp inc/models.hpp inc/scene.hpp
	$(MAKE_OBJ)

obj/test.o: src/test.cpp inc/models.hpp inc/scene.hpp
	$(MAKE_OBJ)

obj/scene.o: src/scene.cpp inc/scene.hpp inc/models.hpp inc/vec3.hpp
	$(MAKE_OBJ)

obj/vec3.o: src/vec3.cpp inc/vec3.hpp
	$(MAKE_OBJ)

clean:
	$(CLEAN)
