// myStack.h using arrays and std::pair<int, int>
#include <iostream>
#include <utility>    // for std::pair
#include <stdexcept>  // for exceptions

class myStack
{
public:
    // Constructor
    myStack(int elements = 1024);
    ~myStack();
    void push(std::pair<int, int>);
    void pop();
    std::pair<int, int> StackTop() const;
    bool IsFull() const;
    bool IsEmpty() const;

    void clear();

private:
    std::pair<int, int>* stack;  // Pointer to the dynamic array
    int top, Maxsize;
};

