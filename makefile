TARGET := assignment2
CLEAN := rm ./obj/*.o ./obj/models/*.o $(TARGET)
CXX := g++

CFLAGS_NIX := -c -Wall -Wextra -std=c++17 -I./inc
CFLAGS_WIN := -c -Wall -Wextra -std=c++17 -I".\inc"

LFLAGS_NIX := -lm -lGL -lGLU -lglut
LFLAGS_WIN := -static -lm -lglut32cu -lglu32 -lopengl32

MODELS_SRC = $(wildcard ./src/models/*.cpp)
MODELS_OBJ = $(patsubst ./src/models/%.cpp, ./obj/models/%.o, $(MODELS_SRC))

MAKE_OBJ = $(CXX) $< -o $@ $(CFLAGS_WIN) 
MAKE_EXE = $(CXX) $^ -o $@ $(LFLAGS_WIN)

$(TARGET): obj/main.o obj/scene.o obj/vec3.o obj/vec4.o obj/material.o obj/material_handler.o \
		obj/path.o obj/texture.o obj/texture_handler.o obj/stb_image.o $(MODELS_OBJ)
	$(MAKE_EXE)

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

obj/vec4.o: src/vec4.cpp inc/vec4.hpp
	$(MAKE_OBJ)

obj/path.o: src/path.cpp inc/path.hpp
	$(MAKE_OBJ)

obj/stb_image.o: src/stb_image.c inc/stb_image.h
	$(MAKE_OBJ)

clean:
	$(CLEAN)
