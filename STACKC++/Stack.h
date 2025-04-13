#pragma once

class Stack
{
private:
    int *data;
    int top;
    int capacity;

    void resize();

public:
    Stack();
    ~Stack();

    void push(int elem);
    int pop();
    bool isEmpty() const;
};
