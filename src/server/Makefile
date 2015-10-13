# test

# Name of binary executable 
outputname = Naviberry
# the object files used
objects = motordriver.o bcmwrapper.o naviberryio.o comm.o server.o serial.o sonicsensor.o scheduler.o cbuffer.o
# compile flags
CFLAGS = -lbcm2835 -std=c++0x -ggdb # -Werror -Wall


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

serial.o:
	g++ -c ./include/serial.cpp  $(CFLAGS)

scheduler.o:	./include/scheduler.cpp ./include/scheduler.hpp
	g++ -c ./include/scheduler.cpp $(CFLAGS)

.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
