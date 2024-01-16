CXX = g++
PATH_INCLUDE = include
PATH_LIBRARY = lib
LIBRARIES = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype

all: main
	del /f main *.o
main: main.o Camera.o Fps.o Map.o Player.o Tools.o World.o
	$(CXX) $^ -o $@ -L ${PATH_LIBRARY} ${LIBRARIES}

main.o:
	$(CXX) -c src/main.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

Camera.o:
	$(CXX) -c src/Camera.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

Fps.o:
	$(CXX) -c src/Fps.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

Map.o:
	$(CXX) -c src/Map.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

Player.o:
	$(CXX) -c src/Player.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

Tools.o:
	$(CXX) -c src/Tools.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

World.o:
	$(CXX) -c src/World.cpp -DSFML_STATIC -I $(PATH_INCLUDE) 

.PHONY: all clean