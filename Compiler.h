#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <map>
#include "Queue.h"
#include "Stack.h"
using namespace std;

class Compiler {
	public:
		Compiler();

		//Methods to turn smp to sml
		void compile(string filename);
		void evaluate(string lineNum, string command, string expression);
		
		//Methods to maintain and store instance variables
		void addVariable(string varName);
		void addLineNum(string lineNum);
		void saveToFile() const;

		//Bool check methods to determine type of string
		bool isVariable(string str) const;
		bool isNumeric(string str) const;
		bool isLetter(string str) const;
		bool isOperator(string str) const;
		bool isRelOp(string str) const;
		bool isCommand(string str) const;

		//Methods to manipulate strings
		string convertToPostFix(string expression, string lineNum);
		vector<string> split(string str);

	private:
		int variablesLeft; 				//represents memory address of variables
		string filename;				//stores name of file
		int index;						//represents memory address of sml instruction
		array<int, 100> memory;			//where sml instructions are stored
		map<string, int> variables;		//map linking variable names to their sml location
		map<string, int> lineNumbers; 	//map linking smp line numbers to their index in memory
};

#endif