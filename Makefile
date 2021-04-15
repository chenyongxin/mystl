CXX = g++
SRC = $(wildcard *.cc)
OBJ = $(SRC:%.cc=%.o)

all: $(OBJ)
	$(CXX) $^

%.o: %.cc
	$(CXX) -c $< 

clean: 
	rm a.out *.o
