CXX = g++						             # The C++ compiler command
CXXFLAGS = -std=c++14 -g -Wall	 # The C++ compiler options (C++14, preserve debug symbols and warn all)
OBJ = src/main.o # List with all of prerrequisites to compile our program
# OBJ2 = src/middleware/middleware.o  

all: smart_lock.out 

smart_lock.out: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

#Remove the object that I generate after get the
#file linked correctlys
clean :
	rm -rf src/*.o src/middleware/*.o *.out test/*.out