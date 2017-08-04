# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -O3 -I/opt/gurobi652/linux64/include/ -L/opt/gurobi652/linux64/lib -lgurobi_c++ -lgurobi65 -std=c++11
DIR = .LKH

all:
	chmod +x LKsolver.sh
	$(MAKE) -C $(DIR)/SRC all
	$(CC) $(DIR)/my-solver.cpp $(CFLAGS) -o $(DIR)/my-solver 
		
clean:
	$(MAKE) -C $(DIR)/SRC clean
