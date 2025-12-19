# Makefile

CXX = g++
CXXFLAGS = -g -Wall -std=c++14

# Main program target
main: main.o riders.o
	$(CXX) $(CXXFLAGS) -o main main.o riders.o

# Test program target
test_suite: tests.o riders.o
	$(CXX) $(CXXFLAGS) -o test_suite tests.o riders.o

# Compilation rules
main.o: main.cpp riders.h
	$(CXX) $(CXXFLAGS) -c main.cpp

riders.o: riders.cpp riders.h
	$(CXX) $(CXXFLAGS) -c riders.cpp

tests.o: tests.cpp riders.h catch.hpp
	$(CXX) $(CXXFLAGS) -c tests.cpp

# Command required by the assignment
check: test_suite
	./test_suite

clean:
	rm -f *.o main test_suite *.dat
