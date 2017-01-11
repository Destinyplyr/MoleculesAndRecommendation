# In order to execute this "makefile" just type "make"

OBJS 	=  mainSample.o CLI.o ClusterNodeFunctions.o ClusterTableFunctions.o DistanceMatrixFunctions.o Utilities.o InitializationMedoids.o AssignmentPAM.o UpdateALaLoyds.o InitializationConcentrate.o UpdateCLARANS.o 
SOURCE	=  mainSample.cpp CLI.cpp ClusterNodeFunctions.cpp ClusterTableFunctions.cpp DistanceMatrixFunctions.cpp Utilities.cpp InitializationMedoids.cpp AssignmentPAM.cpp UpdateALaLoyds.cpp InitializationConcentrate.cpp UpdateCLARANS.cpp 
HEADER  = mainSample.h Headers.h UtilsH.h Algorithms.h ClusterNode.h ClusterTable.h DistanceMatrixFunctions.h ListData.h MathFunctions.h Returns.h TrickList.h
OUT  	= recommendation
CC	= g++
FLAGS   = -g -c -std=c++11

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files

mainSample.o: mainSample.cpp Node.h ListData.h Hash.h ListsFunctions.cpp NodeFunctions.cpp HashFunctions.cpp VectorHammingListManagement.cpp DBListManagement.cpp HammingListManagement.cpp DBHManagement.cpp HashFunctions.cpp HammingLSHManagement.cpp EuclideanList.cpp TrickFunctions.cpp CosineList.cpp MathFunctions.cpp
	$(CC) $(FLAGS) mainSample.cpp

InitializationMedoids.o: InitializationMedoids.cpp
	$(CC) $(FLAGS) InitializationMedoids.cpp

InitializationConcentrate.o: InitializationConcentrate.cpp
	$(CC) $(FLAGS) InitializationConcentrate.cpp	

AssignmentPAM.o: AssignmentPAM.cpp
	$(CC) $(FLAGS) AssignmentPAM.cpp	

UpdateALaLoyds.o: UpdateALaLoyds.cpp
	$(CC) $(FLAGS) UpdateALaLoyds.cpp

UpdateCLARANS.o: UpdateCLARANS.cpp
	$(CC) $(FLAGS) UpdateCLARANS.cpp

Utilities.o: Utilities.cpp
	$(CC) $(FLAGS) Utilities.cpp	

MainFunctions.o: MainFunctions.cpp
	$(CC) $(FLAGS) MainFunctions.cpp

# MathFunctions.o: MathFunctions.cpp
	# $(CC) $(FLAGS) MathFunctions.cpp

DistanceMatrixFunctions.o: DistanceMatrixFunctions.cpp
	$(CC) $(FLAGS) DistanceMatrixFunctions.cpp

ClusterNodeFunctions.o: ClusterNodeFunctions.cpp
	$(CC) $(FLAGS) ClusterNodeFunctions.cpp		

ClusterTableFunctions.o: ClusterTableFunctions.cpp
	$(CC) $(FLAGS) ClusterTableFunctions.cpp	

CLI.o: CLI.cpp
	$(CC) $(FLAGS) CLI.cpp

# clean 
clean:
	rm -f $(OBJS) $(OUT) 

#accounting
count:
	wc $(SOURCE) $(HEADER)