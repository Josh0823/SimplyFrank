#include "Compiler.h"

//Compiles a file written in SimplyFrank code into a file containing sml assembly code
//Needs to be run as ./frankc <filename.fk>
int main(int argc, char** argv) {
	Compiler compiler;
	compiler.compile(argv[1]);

	return 0;
}
