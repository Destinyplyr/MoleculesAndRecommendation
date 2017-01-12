# In order to execute this "makefile" just type "make"

OBJS 	=  mainSample.o Utilities.o DistanceMatrixFunctions.o
SOURCE	=  mainSample.cpp Utilities.cpp DistanceMatrixFunctions.cpp
HEADER  = mainSample.hpp Headers.hpp Returns.hpp Utilities.hpp DistanceMatrixFunctions.hpp
OUT  	= molecules
CC	= g++
FLAGS   = -g -c
LAPACKEGLSLIB = -I"/usr/include" -L"/usr/lib" -llapacke -lblas -lgsl -lgslcblas -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LAPACKEGLSLIB)

# create/compile the individual files
mainSample.o: mainSample.cpp 
	$(CC) $(FLAGS) mainSample.cpp

Utilities.o: Utilities.cpp 
	$(CC) $(FLAGS) Utilities.cpp	

DistanceMatrixFunctions.o: DistanceMatrixFunctions.cpp 
	$(CC) $(FLAGS) DistanceMatrixFunctions.cpp

# clean 
clean:
	rm -f $(OBJS) $(OUT) 

#accounting
count:
	wc $(SOURCE) $(HEADER)