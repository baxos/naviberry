# test

# Name of binary executable 
outputname = Naviberry
# the object files used
objects = motordriver.o bcmwrapper.o naviberryio.o comm.o server.o navi-utils.o sonicsensor.o cbuffer.o mapping.o point.o motorcontroller.o communications.o
# compile flags
CFLAGS = -lbcm2835 -std=c++0x -pthread -O0 -ggdb # -Werror -Wall


Naviberry: $(objects)
	g++ -o $(outputname) $(objects) $(CFLAGS) 
	@echo Binary executable : $(outputname)

motordriver.o:	./include/motordriver.cpp ./include/motordriver.hpp
	g++ -c ./include/motordriver.cpp -o ./motordriver.o $(CFLAGS)

bcmwrapper.o:	./include/bcmwrapper.cpp ./include/bcmwrapper.hpp
	g++ -c ./include/bcmwrapper.cpp  $(CFLAGS)

naviberryio.o:	./include/naviberryio.cpp ./include/naviberryio.hpp
	g++ -c ./include/naviberryio.cpp $(CFLAGS)

server.o:	./server.cpp
	g++ -c ./server.cpp  $(CFLAGS)

cbuffer.o: 	./include/cbuffer.cpp ./include/cbuffer.hpp
	g++ -c ./include/cbuffer.cpp $(CFLAGS)

comm.o:	./include/comm.cpp ./include/comm.hpp
	g++ -c ./include/comm.cpp  $(CFLAGS)

sonicsensor.o:	./include/sonicsensor.cpp ./include/sonicsensor.hpp
	g++ -c ./include/sonicsensor.cpp  $(CFLAGS)

mapping.o:	./include/mapping.cpp ./include/mapping.hpp
	g++ -c ./include/mapping.cpp $(CFLAGS)

point.o:	./include/point.cpp ./include/point.hpp
	g++ -c ./include/point.cpp $(CFLAGS)

motorcontroller.o:	./include/motorcontroller.cpp ./include/motorcontroller.hpp
	g++ -c ./include/motorcontroller.cpp $(CFLAGS)

navi-utils.o:		./include/navi-utils.cpp ./include/navi-utils.hpp
	g++ -c ./include/navi-utils.cpp $(CFLAGS)

communications.o:	./include/communications.cpp ./include/communications.hpp
	g++ -c ./include/communications.cpp $(CFLAGS)

.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
