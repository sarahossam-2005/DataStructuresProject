#include "myStack.h"

myStack::myStack(int elements)
{
    Maxsize = elements;
    stack = new std::pair<int, int>[Maxsize];
    top = -1;
}

myStack::~myStack()
{
    delete[] stack;
}

void myStack::push(std::pair<int, int> x)
{
    if (IsFull())
    {
        throw std::overflow_error("The stack is full");
    }
    else
    {
        stack[++top] = x;
    }
}

void myStack::pop()
{
    if (IsEmpty())
    {
        throw std::underflow_error("The stack is empty");
    }
    else
    {
        top--;
    }
}

std::pair<int, int> myStack::StackTop() const
{
    if (IsEmpty())
    {
        throw std::underflow_error("The stack is empty");
    }
    return stack[top];
}

bool myStack::IsFull() const
{
    return (top >= Maxsize - 1);
}

bool myStack::IsEmpty() const
{
    return (top < 0);
}

void myStack::clear() {
    top = -1;
}