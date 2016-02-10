# test

# Name of binary executable 
outputname = pathfinder
# the object files used
objects = pathfinder.o graphics.o astar.o mapping.o node.o
# compile flags
CFLAGS = -lSDL2 -std=c++11 -pthread -O0 -ggdb -Werror -Wall


Pathfinder: $(objects)
	g++ -o $(outputname) $(objects) $(CFLAGS) 
	@echo Binary executable : $(outputname)
pathfinder.o:	./pathfinder.cpp
	g++ -c ./pathfinder.cpp  $(CFLAGS)
graphics.o:	./include/graphics.cpp ./include/graphics.hpp
	g++ -c ./include/graphics.cpp $(CFLAGS)
astar.o:	./include/astar.cpp ./include/astar.hpp
	g++ -c ./include/astar.cpp $(CFLAGS)
mapping.o:	./include/mapping.cpp ./include/mapping.hpp
	g++ -c ./include/mapping.cpp $(CFLAGS)
node.o:		./include/node.cpp ./include/node.hpp
	g++ -c ./include/node.cpp $(CFLAGS)
.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
