#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <array>
using namespace std;

class Assembler {
	public:
		Assembler();

		void load(string filename); //reads a file into the memory array
		void execute();				//executes the sml code in the memory array

	private:
		array<int, 100> memory;
		int accumulator = 0;

		const int READ = 10;
		const int WRITE = 11;
		const int LOAD = 20;
		const int STORE = 21;
		const int ADD = 30;
		const int SUBTRACT = 31;
		const int DIVIDE = 32;
		const int MULTIPLY = 33;
		const int BRANCH = 40;
		const int BRANCHNEG = 41;
		const int BRANCHZERO = 42;
		const int HALT = 43;
};

#endif