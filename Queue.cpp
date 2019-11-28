#include "Queue.h"

Queue::Queue() {
   
}

//Adds element to the back of the queue
void Queue::enqueue(string element) {
   elements.push_back(element);
}

//Removes and returns element from the front of the queue
string Queue::dequeue() {
   string result = elements[0];
   elements.erase(elements.begin());
   return result;
}

//Returns the number of elements in the queue
int Queue::size() const {
   return elements.size();
}