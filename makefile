CXX = g++
CXXFLAGS = -std=c++11
OBJECTS = Stack.o Queue.o Compiler.o Assembler.o mainC.o mainA.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o frankc Stack.o Queue.o Compiler.o mainC.o
	$(CXX) $(CXXFLAGS) -o frank Assembler.o mainA.o

$(OBJECTS): Stack.h Queue.h Compiler.h Assembler.h
