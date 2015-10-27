# test

# Name of binary executable 
outputname = Navicon
# the object files used
objects = naviberryio.o comm.o navicon.o  cbuffer.o
# compile flags
CFLAGS = -lbcm2835 -std=c++0x -pthread -O0 -ggdb # -Werror -Wall


Navicon: $(objects)
	g++ -o $(outputname) $(objects) $(CFLAGS) 
	@echo Binary executable : $(outputname)
naviberryio.o:	./include/naviberryio.cpp ./include/naviberryio.hpp
	g++ -c ./include/naviberryio.cpp $(CFLAGS)

navicon.o:	./navicon.cpp
	g++ -c ./navicon.cpp  $(CFLAGS)

cbuffer.o: 	./include/cbuffer.cpp ./include/cbuffer.hpp
	g++ -c ./include/cbuffer.cpp $(CFLAGS)

comm.o:	./include/comm.cpp ./include/comm.hpp
	g++ -c ./include/comm.cpp  $(CFLAGS)

.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
