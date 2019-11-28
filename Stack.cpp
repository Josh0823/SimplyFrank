#include "Stack.h"

Stack::Stack() {

}

//Adds an element to top of the stack
void Stack::push(string element) {
   elements.push_back(element);
}

//Removes and returns element at top of the stack
string Stack::pop() {
   string result = elements[elements.size()-1];
   elements.erase(elements.begin() + elements.size()-1);
   return result;
}

//Returns what is currently at top of stack
string Stack::peek() {
	return elements[elements.size()-1];
}

//Returns the number of elements in the stack
int Stack::size() const {
   return elements.size();
}