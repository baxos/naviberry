# test

# Name of binary executable 
outputname = Server
# the object files used
objects = server.o utils.o comm.o status.o bcmwrapper.o motordriver.o
# compile flags
CFLAGS = -lbcm2835 -ggdb # -Werror -Wall

Server: $(objects)
	gcc -o $(outputname) $(objects) $(CFLAGS) 
	@echo Binary executable : $(outputname)
utils.o:
	gcc -c ./include/utils.c $(CFLAGS) -o ./utils.o

comm.o:
	gcc -c ./include/comm.c $(CFLAGS) -o ./comm.o

status.o:
	gcc -c ./include/status.c $(CFLAGS) -o ./status.o

bcmwrapper.o:
	gcc -c ./include/bcmwrapper.c  -o ./bcmwrapper.o $(CFLAGS)

motordriver.o:
	gcc -c ./include/motordriver.c -o ./motordriver.o $(CFLAGS)

server.o:
	gcc -c ./server.c $(CFLAGS) -o ./server.o

.PHONY : clean
clean : 
	$(warning Cleaning object files [ $(objects) ] )
	rm $(objects)

.PHONY : clean
cleanall : clean
	rm $(outputname)
