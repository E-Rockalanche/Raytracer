TARGET := assignment2
CLEAN := del .\obj\*.o .\obj\models\*.o $(TARGET)
CXX := g++

CFLAGS_NIX := -c -Wall -Wextra -std=c++11 -I./inc
CFLAGS_WIN := -c -Wall -Wextra -std=c++11 -I./inc -I"C:\MinGW\glut"

LFLAGS_NIX := -lm -lGL -lGLU -lglut
LFLAGS_WIN := -lm -static -lopengl32 -lglu32 -lglut32 -L"C:\MinGW\glut\lib" -L"C:\MinGW\lib"

MODELS_SRC = $(wildcard ./src/models/*.cpp)
MODELS_OBJ = $(patsubst ./src/models/%.cpp, ./obj/models/%.o, $(MODELS_SRC))

MAKE_OBJ = $(CXX) $(CFLAGS_WIN) $< -o $@
MAKE_EXE = $(CXX) $(LFLAGS_WIN) $^ -o $@

$(TARGET): obj/main.o obj/scene.o obj/vec3.o obj/material.o obj/material_handler.o \
		obj/path.o obj/texture.o obj/texture_handler.o obj/stb_image.o $(MODELS_OBJ)
	$(MAKE_EXE)

wintest.exe: obj/wintest.o obj/scene.o obj/vec3.o obj/material.o obj/material_handler.o \
		obj/path.o obj/texture.o obj/texture_handler.o obj/stb_image.o $(MODELS_OBJ)
	$(MAKE_EXE)

obj/wintest.o: src/wintest.cpp
	$(MAKE_OBJ)

models: $(MODELS_OBJ)

obj/models/%.o: src/models/%.cpp inc/vec3.hpp inc/models.hpp
	$(MAKE_OBJ)

obj/main.o: src/main.cpp inc/models.hpp inc/scene.hpp
	$(MAKE_OBJ)

obj/test.o: src/test.cpp inc/models.hpp inc/scene.hpp
	$(MAKE_OBJ)

obj/material_handler.o: src/material_handler.cpp inc/material_handler.hpp inc/material.hpp
	$(MAKE_OBJ)

obj/texture_handler.o: src/texture_handler.cpp inc/texture_handler.hpp inc/texture.hpp
	$(MAKE_OBJ)

obj/material.o: src/material.cpp inc/material.hpp
	$(MAKE_OBJ)

obj/texture.o: src/texture.cpp inc/texture.hpp
	$(MAKE_OBJ)

obj/scene.o: src/scene.cpp inc/scene.hpp inc/models.hpp inc/vec3.hpp
	$(MAKE_OBJ)

obj/vec3.o: src/vec3.cpp inc/vec3.hpp
	$(MAKE_OBJ)

obj/path.o: src/path.cpp inc/path.hpp
	$(MAKE_OBJ)

obj/stb_image.o: src/stb_image.c inc/stb_image.h
	$(MAKE_OBJ)

clean:
	$(CLEAN)
