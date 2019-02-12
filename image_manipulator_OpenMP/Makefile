CXX=/usr/local/Cellar/gcc/8.1.0/bin/g++-8 -O2 -fopenmp
FLAGS=-W -Wall -I/opt/X11/include -L/opt/X11/lib -lX11 -ljpeg
SRC=filters.cpp main.cpp
OBJ=filters.o main.o
PROG=image_manipulator

all:$(PROG)

image_manipulator: $(OBJ)
	$(CXX) $(FLAGS) -o image_manipulator $^
filters.o: filters.cpp
	$(CXX) -c -o filters.o filters.cpp
main.o: filters.o main.cpp
	$(CXX) -c -o main.o main.cpp
clean:
	rm $(OBJ)
very_clean:
	rm $(OBJ) $(PROG)
	
