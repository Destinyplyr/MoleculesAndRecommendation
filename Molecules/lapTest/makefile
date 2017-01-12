# In order to execute this "makefile" just type "make"

OBJS 	=  svd_demo.o
SOURCE	=  svd_demo.cpp 
HEADER  = 
OUT  	= laptesting
CC	= g++
FLAGS   = -g -c 
LAPACKEGLSLIB = -I"/usr/include" -L"/usr/lib" -llapacke -lblas -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LAPACKEGLSLIB)

# create/compile the individual files

svd_demo.o: svd_demo.cpp 
	$(CC) $(FLAGS) svd_demo.cpp

# clean 
clean:
	rm -f $(OBJS) $(OUT) 

#accounting
count:
	wc $(SOURCE) $(HEADER)