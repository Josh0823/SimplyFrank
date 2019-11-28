#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED
#include <vector>
#include <string>
using namespace std;

//Represents a queue data structure
class Queue {
   public:
      Queue();
      
      void enqueue(string element);
      string dequeue();
      int size() const;        
      
   private:
      vector<string> elements;
};

#endif