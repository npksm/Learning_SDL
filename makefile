# Nick Moller
# nick.pksm@gmail.com

#Variables
CC = g++

CFLAGS = -w 
LINKER_FLAGS = -lSDL2

#Program name variables
PROG1 = SDL_Project
PROG2 = 
PROG3 = 
PROG4 = 
PROG5 = 

INCL1 = 
INCL2 = 

TARGETS = $(PROG1) 
CSRCS = $(PROG1).cpp 

INCLUDES = 

OBJS = $(CSRCS:.cpp=.o)

#Rules
all: $(TARGETS)

% :  
	$(CC) $(CSRCS) $(CFLAGS) $(LINKER_FLAGS) -o $(PROG1)

#Non-build
.PHONY: clean cls git zip

clean cls:
	rm -f $(OBJS) $(TARGETS) *.zip *.out
	rm -f *~ \#*

git:
	git add *.cpp ?akefile
	git commit -m"git make"
	git push
