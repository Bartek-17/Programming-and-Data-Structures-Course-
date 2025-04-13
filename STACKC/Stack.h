#pragma once
#include <stdbool.h>

typedef struct
{
    int *data; // Pointer to dynamically allocated array
    int top;
    int capacity;
} Stack;

void init(Stack *s);
void destroy(Stack *s);
void push(Stack *s, int elem);
int pop(Stack *s);
bool isEmpty(const Stack *s);
