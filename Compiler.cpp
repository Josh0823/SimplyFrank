#include "Compiler.h"

//Default constructor that initializes memory to 0, index to 0, and variablesLeft to the upper index of memory
Compiler::Compiler() {
	for (int i = 0; i < memory.size(); i++) {
		memory[i] = 0;
	}
	variablesLeft = memory.size() - 1;
	index = 0;
}

//Reads in a file of smp code and calls evaluate on each line
void Compiler::compile(string filename) {
	//Gets the extension of the file
	string filetype = filename.substr(filename.length() - 3);

	//Throws an error if it is not a .frank file
	if (filetype != ".fk") {
		cerr << "FILE ERROR" << endl;
		cerr << "Ensure filetype is \".fk\"" << endl;
		exit(EXIT_FAILURE);
	}
	this->filename = filename.substr(0, filename.length()-3);

	//Opens file with simple code
	ifstream fin;
	fin.open(filename);

	//Throws an error if the file fails to open
	if (fin.fail()) {
		cerr << "FILE ERROR" << endl;
		cerr << "Failed to open \"" << filename << "\"" << endl;
		exit(EXIT_FAILURE);
	}

	//Each line of smp code has a lineNum, command, and expression
	string lineNum, command, expression;
	bool endFound = false;
	
	//Reads in the lineNum, command, and expression
	//Throws a syntax error if anything goes wrong while reading in file
	while (!fin.eof() && command != "end") {

		fin >> lineNum;
		if (lineNum.length() > 2 || !isNumeric(lineNum)) {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid line number \"" << lineNum << "\"" << endl;
			exit(EXIT_FAILURE);
		}


		try {
			fin >> command;

		} catch(...) {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Missing command on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}

		try {
			if (command == "end") {
				endFound = true;
				expression = "00";
			} 
			else {
				getline(fin, expression);
				expression = expression.substr(1, expression.length());
			}
		} catch(...) {
			cerr << "SYNATX ERROR" << endl;
			cerr << "Missing expression (line " << lineNum << ")" << endl;
			exit(EXIT_FAILURE);
		}
		
		//turns the simple line of code into sml code and resets line variables
		evaluate(lineNum, command, expression);
		lineNum, command, expression = "";
	}

	//Error thrown if end is not found
	if (!endFound) {
		cerr << "SYNTAX ERROR" << endl;
		cerr << "Program must terminate with \"end\" command" << endl;
		exit(EXIT_FAILURE);
	}

   //Second pass that replaces goto lineNum with actual sml lineNum
	for (int i = 0; i < memory.size(); i++) {
		if (to_string(memory[i]).length() > 2) {
			int opcode = stoi(to_string(memory[i]).substr(0, 2));
			int location = stoi(to_string(memory[i]).substr(2));
			if (opcode == 40 || opcode == 41 || opcode == 42) {
				if (lineNumbers.find(to_string(location)) != lineNumbers.end()) {
					
					location = lineNumbers[to_string(location)];
					memory[i] = opcode*100 + location;
				}
				else
				{
					cerr << "SYNTAX ERROR" << endl;
					cerr << "Undefined line number \"goto " << location << "\"" << endl;
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	//Checks to see if assembler still has memory left
	if (variablesLeft == index || variablesLeft <= 0 || index >= memory.size()) {
		cerr << "MEMORY ERROR" << endl;
		cerr << "Memory is full. Shorten program" << endl;
		exit(EXIT_FAILURE);
	}

	//Saves the created sml code into a .sml file
	saveToFile();
}

//Turns a line of smp code into sml code in the memory array
void Compiler::evaluate(string lineNum, string command, string expression) {
	if (command == "rem") {
		//Ignores comments
	}
	else if (command == "input") {
   		//Adds the variableName to list of variables
		if (isLetter(expression.substr(0, 1))) {
			addVariable(expression);
		}
		else {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid variable name \"" << expression << "\" on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}
		//Gets sml address of variable
		//Inputs value of variable into that address
		int variableLocation = variables[expression];
		memory[index] = 1000 + variableLocation;
		addLineNum(lineNum);
		index++;
	}
	else if (command == "print") {
		addLineNum(lineNum);
		vector<string> expressionVector = split(expression);
		expression = expressionVector[0];

      	//Allows for simple literals to be printed by adding them as a variable
		if (isNumeric(expression)) {
			addVariable(expression);
		}

      	//Ensures variable is not undefined, gets address if defined
		int variableLocation;
		if (variables.find(expression) != variables.end()) {
			variableLocation = variables[expression];
		}
		else {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Undefined variable \"" << expression << "\" on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}
		
      	//Prints value of variable at that address
		memory[index] = 1100 + variableLocation;
		index++;
	}
	else if (command == "let") {
		//Creates a vector based on the equation
		//i.e. "x = 1 + 2" -> ["x", "=", "1", "+", "2"]
		vector<string> equationVector = split(expression);
		
		//Stores first token as variableName and adds it to variables map
		string variableName = equationVector[0];
		if (isLetter(variableName.substr(0, 1)) || variableName.substr(0, 1) == "_") {
			addVariable(variableName);
		}
		else {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid variable name \"" << variableName << "\" on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}

		if (equationVector[1] != "=") {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid assignment operator \"" << equationVector[1] << "\" on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}


		//Stores rest of expression (except equals sign) as the equation
		string equation = "";
		for (int i = 2; i < equationVector.size(); i++) {
			equation = equation + " " + equationVector[i];
		}
		if (equation.substr(0, 1) == " ") {
			equation = equation.substr(1, equation.length() - 1);
		}


		//Creates a vector from equation
		vector<string> numbers = split(equation);

		//For each token in vector, if token is a number, added as a variable
		for (int i = 0; i < numbers.size(); i++) {
			if (isNumeric(numbers[i]))
				addVariable(numbers[i]);
		}

		//Calls method to turn equation into postfix
		equation = convertToPostFix(equation, lineNum);
		
		vector<string> postfixVector = split(equation);

		//It evaluates the postfix equation and stores it into variableName
		addLineNum(lineNum);
		int j = 0;
		for (int i = 0; i < postfixVector.size(); i++) {
			//Adds any literals that have not been encountered before
			if (isNumeric(postfixVector[i])) {
				addVariable(postfixVector[i]);
			}
			
         if (isOperator(postfixVector[i])) {
				//Loads the variable/literal two places ahead in the postfix expression
				memory[index] = 2000 + variables[postfixVector[i-2]];
				index++;

				//Assigns opcode based on the operator
				int opcode;
				if (postfixVector[i] == "+") opcode = 3000;
				if (postfixVector[i] == "-") opcode = 3100;
				if (postfixVector[i] == "*") opcode = 3300;
				if (postfixVector[i] == "/") opcode = 3200;

				//Performs the mathematic expression with the variable/literal one place ahead in the postfix expression
				memory[index] = opcode + variables[postfixVector[i-1]];
				index++;

				//Removes the two operands and replaces with a variable name
				for (int k = 0; k <= 2; k++) {
					postfixVector.erase(postfixVector.begin() + i - k);
				}
				int variableLocation = variablesLeft;
				postfixVector.insert(postfixVector.begin() + i - 2, "_" + to_string(variableLocation));
				addVariable("_" + to_string(variableLocation));

				//Stores the result of the 1 math phrase into a variable in sml
				memory[index] = 2100 + variables["_" + to_string(variableLocation)];
				index++;

				//Resets i to 0
				i = 0;
			}
		}
		//Loads the final result and stores it into the variable name
		memory[index] = 2000 + variables[postfixVector[0]];
		index++;

		memory[index] = 2100 + variables[variableName];
		index++;
	}
	else if (command == "goto") {
		//Places a branch opcode with the location in smp to branch to
		//Smp location will be replaced with sml index during second pass
		if (stoi(expression) > 99 || stoi(expression) < 0) {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid goto expression on line " << lineNum << endl;
			exit(EXIT_FAILURE); 
		}
		else 
		{
			memory[index] = 4000 + stoi(expression);
			addLineNum(lineNum);
			index++;
		}
	}
	else if (command == "if") {
		//Splits the expression into a vector
		vector<string> tokens = split(expression);

		//Gets the left side of conditional statement
		//i.e. x > 0 -> x
		int i;
		string leftExpression;
		for (i = 0; i < tokens.size(); i++) {
			if (isRelOp(tokens[i]))
				break;
			leftExpression = leftExpression + " " + tokens[i];
		}
		leftExpression = leftExpression.substr(1);
		string relop = tokens[i];


		//Gets the right side of the conditional statement
		//i.e. x > 0 -> 0
		string rightExpression;
		for (i = i+1; i < tokens.size(); i++) {
			if (!isNumeric(tokens[i]) && !isVariable(tokens[i]))
				break;
			rightExpression = rightExpression + " " + tokens[i];
		}
		rightExpression = rightExpression.substr(1);

		//Ensures the conditional command is valid
		if (!isCommand(tokens[i])) {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid command \"" << tokens[i] << "\" on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}

		//Gets the command to be executed if the statement is true
		string nextCommand = tokens[i];
		string nextExpression;
		for (int j = i+1; j < tokens.size(); j++) {
			nextExpression = nextExpression + " " + tokens[j];
		}
		nextExpression = nextExpression.substr(1);

		//Creates two variables to evaluate the left and right expression
		string leftLocation = to_string(variablesLeft);
		evaluate("_", "let", "_" + leftLocation + " = " + leftExpression);
		
		string rightLocation = to_string(variablesLeft);
		evaluate("_", "let", "_" + rightLocation + " = " + rightExpression);

		//Checks to see if the statement if true using the two created variables
		if (relop == ">") {
			memory[index] = 2000 + variables["_" + rightLocation];
			addLineNum(lineNum);
			index++;

			memory[index] = 3100 + variables["_" + leftLocation];
			index++;

			memory[index] = stoi("41" + to_string(100 + index + 2));
			index++;
		}
		else if (relop == ">=") {
			memory[index] = 2000 + variables["_" + rightLocation];
			addLineNum(lineNum);
			index++;

			memory[index] = 3100 + variables["_" + leftLocation];
			index++;

			memory[index] = stoi("41" + to_string(100 + index + 3));
			index++;

			memory[index] = stoi("42" + to_string(100 + index + 2));
			index++;
		}
		else if (relop == "<") {
			memory[index] = 2000 + variables["_" + leftLocation];
			addLineNum(lineNum);
			index++;

			memory[index] = 3100 + variables["_" + rightLocation];
			index++;

			memory[index] = stoi("41" + to_string(100 + index + 2));
			index++;
		}
		else if (relop == "<=") {
			memory[index] = 2000 + variables["_" + leftLocation];
			addLineNum(lineNum);
			index++;

			memory[index] = 3100 + variables["_" + rightLocation];
			index++;

			memory[index] = stoi("41" + to_string(100 + index + 3));
			index++;

			memory[index] = stoi("42" + to_string(100 + index + 2));
			index++;
		} 
		else if (relop == "==") {
			memory[index] = 2000 + variables["_" + leftLocation];
			addLineNum(lineNum);
			index++;

			memory[index] = 3100 + variables["_" + rightLocation];
			index++;

			memory[index] = stoi("42" + to_string(100 + index + 2));
			index++;
		}
		else if (relop == "!=" || relop == "<>") {
			memory[index] = 2000 + variables["_" + leftLocation];
			addLineNum(lineNum);
			index++;

			memory[index] = 3100 + variables["_" + rightLocation];
			index++;

			memory[index] = stoi("41" + to_string(100 + index + 5));
			index++;

			memory[index] = 2000 + variables["_" + rightLocation];
			index++;

			memory[index] = 3100 + variables["_" + leftLocation];
			index++;

			memory[index] = stoi("41" + to_string(100 + index + 2));
			index++;
		}
		else {
			cerr << "SYNTAX ERROR" << endl;
			cerr << "Invalid conditional operator \"" << relop << "\" on line " << lineNum << endl;
			exit(EXIT_FAILURE);
		}
		
		//Gets the current index, stores a blank branch code for now
		int currentIndex = index;
		memory[currentIndex] = 4000 + 00;
		index++;
		
		//Adds the next command into memory
		evaluate(to_string(100 + index), nextCommand, nextExpression);
		
		//Replaces the previous location for the branch code to skip the command above if the expression is false
		string newLineNum = to_string(100 +  index);
		addLineNum(newLineNum);
		memory[currentIndex] = stoi("40" + newLineNum);
	}
	else if (command == "end") {
		//Places a termination opcode in sml
		memory[index] = 4300;
		addLineNum(lineNum);
		index++;
	}
	else {
      //Throws error when an invalid command is entered
		cerr << "SYNTAX ERROR" << endl;
		cerr << "\"" << command << "\" (line " << lineNum << ") is not a recognized command" << endl;
		exit(EXIT_FAILURE);
	}
}

//Links the variableName to a location in memory
void Compiler::addVariable(string variableName) {
	//Checks to see if the variable is actually a numeric literal
	if (isNumeric(variableName)) {
		//If the number hasn't been added yet, it is added to memory
		if (variables.find(variableName) == variables.end()) {
			memory[variablesLeft] = stoi(variableName);
			variables[variableName] = variablesLeft;
			variablesLeft--;
		}
	}
	else {
		//Adds string variable to variable map if it hasn't been added before
		if (variables.find(variableName) == variables.end()) {
			variables[variableName] = variablesLeft;
			variablesLeft--;
		}
	}
}

//Links the smp address to the actual sml index
void Compiler::addLineNum(string lineNum) {
	//If statement ensures no duplicate line numbers
	if (lineNumbers.find(lineNum) == lineNumbers.end() || lineNum == "_") {
		//Maps smp lineNum to the actual sml index
		lineNumbers[lineNum] = index;
	}
	else {
		cerr << "SYNTAX ERROR" << endl;
		cerr << "Duplicate line number \"" << lineNum << "\"" << endl;
		exit(EXIT_FAILURE);
	}
}

//Saves memory array as a .sml file
void Compiler::saveToFile() const {
	ofstream fout;
	fout.open(this->filename + ".fclass");

	for (int i = 0; i < memory.size(); i++) {
		fout << memory[i] << endl;
	}
	fout.close();
}

//Converts an infix expression to postfix
string Compiler::convertToPostFix(string expression, string lineNum) {
	//Adds spaces between numbers/variables and operators so that the expression can be split
	//i.e. (1+x)*(1-y) -> ( 1 + x ) * ( 1 - y )
	for (int i = 0; i < expression.length(); i += 1) {
		//Adds a space after operators and parenthesis
		if (isOperator(expression.substr(i, 1)) || expression.substr(i, 1) == ")" || expression.substr(i,1) == "(") {
			//Ensures that spaces aren't added after a negative sign
			if (expression.substr(i, 1) == "-" && !isNumeric(expression.substr(i+1, 1))) {			
				expression = expression.substr(0, i+1) + " " + expression.substr(i+1, expression.length() - (i+1));
				i++;
			}
			//Adds a space after every other type of operator
			else if (expression.substr(i, 1) != "-") {
				expression = expression.substr(0, i+1) + " " + expression.substr(i+1);
				i++;
			}
		}
		//Adds a space after the end of a variable name
		if (i+1 <= expression.length() && isLetter(expression.substr(i, 1)) && !isLetter(expression.substr(i+1, 1))) {
			expression = expression.substr(0, i+1) + " " + expression.substr(i+1);
			i++;
		}

		if (i+1 <= expression.length() && isNumeric(expression.substr(i, 1)) && !isNumeric(expression.substr(i+1, 1))) {
			expression = expression.substr(0, i+1) + " " + expression.substr(i+1);
			index++;
		}
	}

	//Splits the expression into a vector and creates the stack and queue we use in our algorithm
	vector<string> tokens = split(expression);
	Queue postfix;
	Stack process;

	//Algorithim that uses stacks and queues to convert to postfix like in calculator from Data Structures
	for (int i = 0; i < tokens.size(); i++) {
		if (isVariable(tokens[i])) {
			postfix.enqueue(tokens[i]);
		}
		else if (isNumeric(tokens[i])) {
			addVariable(tokens[i]);
			postfix.enqueue(tokens[i]);
		}
		else if (tokens[i] == "(") {
			process.push(tokens[i]);
		}
		else if (isOperator(tokens[i])) {
			if (process.size() > 0 && (tokens[i] == "*" || tokens[i] == "/") && (process.peek() == "*" || process.peek() == "/")) {
				postfix.enqueue(process.pop());
			}
			else if (process.size() > 0 && (tokens[i] == "+" || tokens[i] == "-") && isOperator(process.peek())) {
				postfix.enqueue(process.pop());
			}
			process.push(tokens[i]);
		}
		else if (tokens[i] == ")") {
			while (process.peek() != "(") {
				postfix.enqueue(process.pop());
			}
			process.pop();
		}
      else {
         cerr << "SYNTAX ERROR" << endl;
         cerr << "Undefined symbol \"" << tokens[i] << "\" on line " << stoi(lineNum)%100 << endl;
         exit(EXIT_FAILURE);
      }
    }

	//This empties the process stack and places all its elements at the back of the queue
	while (process.size() > 0) {
		postfix.enqueue(process.pop());
	}

	//We then dequeue each element in the postfix queue to get our postfix expression
	string result = postfix.dequeue();
	while (postfix.size() > 0) {
		result = result + " " + postfix.dequeue();
	}
	return result;
}

//Checks to see if a string is a variableName somewhere in the map
bool Compiler::isVariable(string str) const {
	if (variables.find(str) != variables.end())
		return true;
	else
		return false;
}

//Checks to see if a string is a number
bool Compiler::isNumeric(string str) const {
	if (str.substr(0, 1) == "-" && str.length() > 1)
		str = str.substr(1);

	for (int i = 0; i < str.length(); i++) {
		try {
			int num = stoi(str.substr(i, 1));
		} catch(...) {
			return false;
		}
	}
	return true;
}

//Checks to see if a string of length 1 is a letter
bool Compiler::isLetter(string str) const {
	string letters[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
	for (int i = 0; i < 26; i++) {
		if (str == letters[i])
			return true;
	}
	return false;
}

//Checks to see if a string is an approved math operator
bool Compiler::isOperator(string str) const {
	return (str == "+" || 
			str == "-" || 
			str == "*" || 
			str == "/");
}

//Checks to see if a string is an approved relational operator
bool Compiler::isRelOp(string str) const {
	return (str == ">=" || 
			str == "<=" || 
			str == "==" || 
			str == "!=" ||
			str == ">" || 
			str == "<" ||  
			str == "<>");
}

//Checks to see if a string is an approved command
bool Compiler::isCommand(string str) const {
	return (str == "input" || 
			str == "print" || 
			str == "let"   || 
			str == "if"    || 
			str == "goto"  || 
			str == "end");
}

//Splits a string into a vector with spaces as the delimeter
//i.e "Hello World" -> ["Hello", "World"]
vector<string> Compiler::split(string str) {
	vector<string> tokens;
	string word = "";
	for (int i = 0; i < str.length(); i++) {
		if (str.substr(i, 1) != " ") {
			word = word + str.substr(i, 1);
		}
		else {
			if (word != "") {
				tokens.push_back(word);
				word = "";
			}
		}
	}
	if (word != "")
		tokens.push_back(word);
	return tokens;
}