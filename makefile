TARGET := assignment2
CXX := g++

SDL_LFLAGS := -Wl,-Bdynamic -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -Wl,-Bstatic
LFLAGS := -std=c++17 -O3 -lmingw32 -lm -mwindows -mconsole $(SDL_LFLAGS)
CLEAN := del .\obj\*.o .\obj\models\*.o $(TARGET)

CFLAGS := -c -O3 -Wall -Wextra -std=c++17 -I./src -I./src/models -I./lib

MODELS_SRC = $(wildcard ./src/models/*.cpp)
MODELS_OBJ = $(patsubst ./src/models/%.cpp, ./obj/models/%.o, $(MODELS_SRC))

MAKE_OBJ = $(CXX) $< -o $@ $(CFLAGS)
MAKE_EXE = $(CXX) $^ -o $@ $(LFLAGS)

$(TARGET): obj/main.o obj/scene.o obj/material.o obj/material_handler.o \
		obj/texture.o obj/texture_handler.o obj/stb_image.o $(MODELS_OBJ)
	$(MAKE_EXE)

models: $(MODELS_OBJ)

obj/models/%.o: src/models/%.cpp src/models/%.hpp src/math/vec3.hpp
	$(MAKE_OBJ)

obj/main.o: src/main.cpp src/models/model.hpp src/scene.hpp
	$(MAKE_OBJ)

obj/material_handler.o: src/material_handler.cpp src/material_handler.hpp src/material.hpp
	$(MAKE_OBJ)

obj/texture_handler.o: src/texture_handler.cpp src/texture_handler.hpp src/texture.hpp
	$(MAKE_OBJ)

obj/material.o: src/material.cpp src/material.hpp src/math/vec3.hpp
	$(MAKE_OBJ)

obj/texture.o: src/texture.cpp src/texture.hpp src/math/vec4.hpp
	$(MAKE_OBJ)

obj/scene.o: src/scene.cpp src/scene.hpp src/models/model.hpp src/math/vec3.hpp src/math/vec4.hpp
	$(MAKE_OBJ)

obj/stb_image.o: src/stb_image.c src/stb_image.h
	$(MAKE_OBJ)


clean:
	$(CLEAN)
