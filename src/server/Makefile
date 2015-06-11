# test

# Name of binary executable 
outputname = Server
# the object files used
objects = server.o utils.o comm.o status.o bcmwrapper.o motordriver.o serial.o
# compile flags
CFLAGS = -lbcm2835 -ggdb # -Werror -Wall

Server: $(objects)
	gcc -o $(outputname) $(objects) $(CFLAGS) 
	@echo Binary executable : $(outputname)
utils.o:
	gcc -c ./include/utils.cpp $(CFLAGS) -o ./utils.o

comm.o:
	gcc -c ./include/comm.cpp $(CFLAGS) -o ./comm.o

status.o:
	gcc -c ./include/status.cpp $(CFLAGS) -o ./status.o

bcmwrapper.o:
	gcc -c ./include/bcmwrapper.cpp  -o ./bcmwrapper.o $(CFLAGS)

motordriver.o:
	gcc -c ./include/motordriver.cpp -o ./motordriver.o $(CFLAGS)

serial.o:
	gcc -c ./include/serial.cpp -o ./serial.o

server.o:
	gcc -c ./server.c $(CFLAGS) -o ./server.o

.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
