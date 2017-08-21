#init
#### info
#
# 'make'        build executable file 'santa'
# 'make clean'  removes all .o and executable files
#
CC=gcc
CFLAGS = -std=gnu99 -lpthread -Wall  -pedantic  -Wextra -Werror 
OBJ = main.o other_functions.o santa.o elf.o 
HEADERS = baseinfo.h 

%.o : %.c $(HEADERS)
	$(CC)  -c -o $@ $< $(CFLAGS)

santa   : $(OBJ)
	$(CC)  -o $@ $^ $(CFLAGS) 
	
clean :
	@rm -f *.o *~ *.out santa 
