#include "Assembler.h"

//Executes a file of sml assembly code
//Needs to be run as ./frank <filename>
int main(int argc, char** argv) {
	string filename = argv[1];
	filename = filename + ".fclass"; 
	
	Assembler assembler;
	assembler.load(filename); 
	assembler.execute();

	return 0;
}