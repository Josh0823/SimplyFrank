#include "Assembler.h"

//Constructs an assembler object
Assembler::Assembler() {
	//sets all values of memory to 0
	for (int i = 0; i < memory.size(); i++) {
		memory[i] = 0;
	}
}

//Reads from the file and inputs into memory
void Assembler::load(string filename) {
	//Opens file
	ifstream fin;
	fin.open(filename);

	//Exits if the file fails to open
	if (fin.fail()) {
		cerr << "There was an error opening " + filename << endl;
		exit(EXIT_FAILURE);
	}

	int index = 0;				//location in memory to place instruction
	string word = "";			//a single command from the file
	bool haltNotFound = true;	//indicates whether a halt opcode is found

	//Loops through the entire file
	for (int i = 0; i < memory.size(); i++) {
		fin >> word;
		memory[index] = stoi(word);

		if (word == "4300") {
			haltNotFound = false;
		}

		//If the input file is too big give an error
		if (index > memory.size()) {
			cerr << "MEMORY ERROR:" << endl;
			cerr << "Ensure program is smaller than memory allocated" << endl;
			exit(EXIT_FAILURE);
		}
		index++;
	}
	

	//Gives an error message if no halt opcode is found
	if (haltNotFound) {
		cerr << "TERMINATION ERROR" << endl;
		cerr << "Ensure program terminates with \"end\"" << endl;
		exit(EXIT_FAILURE);
	}
}

//Executes code that has been loaded into memory
void Assembler::execute() {
	int index = 0; 
	int opcode, location;

	while(true) {
		location = memory[index] % 100;	//gets last two numbers
		opcode = memory[index] / 100;	//gets first two numbers

		//Read and Write code
		if (opcode == READ) {
			//Reads in variable into memory
			cout << ">> ";
			cin >> memory[location];
		}
		if (opcode == WRITE) {
			//Displays a variable from memory
			cout << memory[location] << endl;
		}
		
		//Load and Store code
		if (opcode == LOAD) {
			//Places a variable from memory into the accumulator
			accumulator = memory[location];
		}
		if (opcode == STORE) {
			//Stores the value of the accumulator into memory
			memory[location] = accumulator;
		}

		//Math code
		if (opcode == ADD) {
			//Adds a variable in memory to the accumulator
			accumulator += memory[location];
		}
		if (opcode == SUBTRACT) {
			//Subttacts a variable in memory from the accumulator
			accumulator -= memory[location];
		}
		if (opcode == DIVIDE) {
			//Divides the accumulator by a variable in memory
			accumulator /= memory[location];
		}
		if (opcode == MULTIPLY) {
			//Multiplies the accumulator by a variable in memory
			accumulator *= memory[location];
		}

		//Branching code
		if (opcode == BRANCH) {
			//Branches directly to a line #
			index = location - 1;
		}
		if (opcode == BRANCHNEG) {
			//Only branches to a line if the accumulator is negative
			if (accumulator < 0) {
				index = location - 1;
			}
		}
		if (opcode == BRANCHZERO) {
			//Only branches to a line # if the accumulator is 0
			if (accumulator == 0) {
				index = location - 1;
			}
		}

		//Halt code, exits from function
		if (opcode == HALT) {
			return;
		}

		index++;
	}
}