//myStack class .h using arrays
#include<iostream>
using namespace std;

template< class T>
class myStack
{
public:
	//constructor
	myStack(int elements = 128);
	~myStack();
	void push(T);
	void pop();
	T StackTop() const;
	bool IsFull() const;
	bool IsEmpty() const;

private:
	T* stack; //pointer to the dynamic array
	int top, Maxsize;

};

template<class T>
myStack<T>::myStack(int elements)
{
    Maxsize = elements;
    stack = new T[Maxsize];
    top = -1;
}

template<class T>
myStack<T>::~myStack()
{
    delete[] stack;
}

template<class T>
void myStack<T>::push(T x)
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

template<class T>
void myStack<T>::pop()
{
    T x;
    if (IsEmpty())
    {
        throw std::underflow_error("The stack is empty");
    }
    else
    {
        x = stack[top--];
    }
}

template <class T>
T myStack<T>::StackTop() const
{
    T x;
    if (IsEmpty())
    {
        throw std::underflow_error("The stack is empty");
    }
    else
    {
        x = stack[top];
    }
    return x;
}

template<class T>
bool myStack<T>::IsFull() const
{
    return (top >= Maxsize - 1);
}

template<class T>
bool myStack<T>::IsEmpty() const
{
    return (top < 0);
}