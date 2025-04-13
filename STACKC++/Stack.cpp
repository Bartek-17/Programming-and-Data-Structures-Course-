#include <iostream>
#include "Stack.h"

#define INITIAL_CAPACITY 4
#define GROWTH_CONST 2

Stack::Stack()
{
    capacity = INITIAL_CAPACITY;
    top = -1;
    data = new int[capacity];
    std::cout << "Constructor called\n";
}

Stack::~Stack()
{
    delete[] data;
    data = nullptr;
    std::cout << "Destructor called\n";
}

void Stack::resize()
{
    capacity *= GROWTH_CONST;
    int *newData = new int[capacity];

    for (int i = 0; i <= top; i++)
    {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
}

void Stack::push(int elem)
{
    if (top == capacity - 1)
    {
        resize();
    }
    top++;
    data[top] = elem;
}

int Stack::pop()
{
    if (isEmpty())
    {
        std::cout << "Error: Cannot pop from an empty stack" << std::endl;
        exit(EXIT_FAILURE);
    }
    int topElement = data[top];
    top--;
    return topElement;
}

bool Stack::isEmpty() const
{
    return top == -1;
}