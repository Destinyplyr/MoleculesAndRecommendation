# In order to execute this "makefile" just type "make"

OBJS 	=  mainSample.o
SOURCE	=  mainSample.cpp 
HEADER  = mainSample.hpp Headers.hpp
OUT  	= molecules
CC	= g++
FLAGS   = -g -c
LAPACKEGLSLIB = -I"/usr/include" -L"/usr/lib" -llapacke -lblas -lgsl -lgslcblas -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LAPACKEGLSLIB)

# create/compile the individual files

mainSample.o: mainSample.cpp 
	$(CC) $(FLAGS) mainSample.cpp

# clean 
clean:
	rm -f $(OBJS) $(OUT) 

#accounting
count:
	wc $(SOURCE) $(HEADER)