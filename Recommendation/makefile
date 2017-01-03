# In order to execute this "makefile" just type "make"

OBJS 	=  mainSample.o CLI.o
SOURCE	=  mainSample.cpp CLI.cpp
HEADER  = mainSample.hpp Headers.hpp
OUT  	= recommendation
CC	= g++
FLAGS   = -g -c 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files

mainSample.o: mainSample.cpp Node.h ListData.h Hash.h ListsFunctions.cpp NodeFunctions.cpp VectorHammingListManagement.cpp DBListManagement.cpp HammingListManagement.cpp DBHManagement.cpp HashFunctions.cpp HammingLSHManagement.cpp EuclideanList.cpp TrickFunctions.cpp CosineList.cpp MathFunctions.cpp
	$(CC) $(FLAGS) mainSample.cpp


Utilities.o: Utilities.cpp
	$(CC) $(FLAGS) Utilities.cpp	

MainFunctions.o: MainFunctions.cpp
	$(CC) $(FLAGS) MainFunctions.cpp

# MathFunctions.o: MathFunctions.cpp
	# $(CC) $(FLAGS) MathFunctions.cpp

DistanceMatrixFunctions.o: DistanceMatrixFunctions.cpp
	$(CC) $(FLAGS) DistanceMatrixFunctions.cpp


CLI.o: CLI.cpp
	$(CC) $(FLAGS) CLI.cpp

# clean 
clean:
	rm -f $(OBJS) $(OUT) 

#accounting
count:
	wc $(SOURCE) $(HEADER)