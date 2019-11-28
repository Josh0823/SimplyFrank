xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

README!

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Welcome to our language, SimplyFrank! Our developers at Frankie's Fan Club Inc. have worked tirelessly to present you with an easy to read, easy to write, easy to modify language (we <3 u deg). Make sure to read through this guide before attempting to write anything, and hopefully we can promise minimal errors and/or core dumps!

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Syntax:

The general syntax of a line in SimplyFrank follows the following format:
	lineNumber command expression

The following are valid commands in SimplyFrank:
	rem - 	adds a comment to the code, expression can be any mix of strings and numbers, it doesnt appear anywhere. (Ex. 10 rem hello 123 this doesnt matter)

	input - creates a variable and generates a command for the user to enter a value to be stored in said variable. Input is limited to just numbers, but variable names can be strings. (Ex. 40 input xBoi)
	
	print - tells the computer to print a specified literal or previously defined variable value. (Ex. 23 print x OR 23 print 5)
	
	let - this is a variable assignment command. It takes a value or expression and stores it inside a given variable. (Ex. 45 let x = 7 OR 45 let x = (x+y)-(x/5))
	
	if - this tests a given conditional statement, and if it is true, evaluates the statement after. (Ex. 45 if x > 5 goto 34 OR 43 if x < 5 let x = 57*x OR 36 if x<0 end run)
	
	goto - 	sends the computer to a specified line number that exists either before or after the line in the code. (Ex. 60 goto 43)
		
	end run- closes the program, it is necessary for this to be at the end of EVERY program! (Ex. 50 end run)

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Common Known Errors / Helpful Tips
	- files must have .fk as the file extension
	- compile files by typing './frankc <filename.fk>'
	- execute the assembly by then typing './frank <filename>'
	- when writing an if statement, putting parenthesis around the statement (Ex. if (x>4)) will generate an error
	- order of operations works for let commands
	- using the go to command, you cannot tell the program to go to a variable storing a line number, you must only use numbers
	- and/or commands are not valid in conditional statements
	- do NOT yell at the computer if your code doesn't do what you want, it has feelings, and Frankie will cry. 
	- really though infinite loops are still possible to create pls don't try it

xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Josh Geden & Caitlin Harrington