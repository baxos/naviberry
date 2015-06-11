# test

# Name of binary executable 
outputname = Test
# the object files used
objects = motordriver.o bcmwrapper.o naviberryio.o comm.o server.o serial.o sonicsensor.o
# compile flags
CFLAGS = -lbcm2835 -std=c++0x -ggdb # -Werror -Wall


Test: $(objects)
	g++ -o $(outputname) $(objects) $(CFLAGS) 
	@echo Binary executable : $(outputname)
motordriver.o:
	g++ -c ./include/motordriver.cpp -o ./motordriver.o $(CFLAGS)

bcmwrapper.o:
	g++ -c ./include/bcmwrapper.cpp -o ./bcmwrapper.o $(CFLAGS)
naviberryio.o:
	g++ -c ./include/naviberryio.cpp -o ./naviberryio.o $(CFLAGS)
server.o:
	g++ -c ./server.cpp -o ./server.o $(CFLAGS)
comm.o:
	g++ -c ./include/comm.cpp  $(CFLAGS)
sonicsensor.o:
	g++ -c ./include/sonicsensor.cpp -o ./sonicsensor.o $(CFLAGS)
serial.o:
	g++ -c ./include/serial.cpp -o ./serial.o $(CFLAGS)

.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
