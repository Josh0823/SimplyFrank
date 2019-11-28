#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <vector>
#include <string>
using namespace std;


//Class that represents a stack data structure
class Stack {
   public:
      Stack();
      
      void push(string element);
      string pop();
      string peek();
      int size() const;
   
   private:
      vector<string> elements;
   
};

#endif