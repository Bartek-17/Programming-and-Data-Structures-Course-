#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

#define INITIAL_CAPACITY 4
#define GROWTH_CONST 2 // Doubles the capacity of stack when space is needed

void init(Stack *s)
{
    s->capacity = INITIAL_CAPACITY;
    s->top = -1;
    s->data = malloc(s->capacity * sizeof(int));
    if (s->data == NULL)
    {
        printf("Error: Cannot allocate memory for the stack.\n");
        exit(EXIT_FAILURE);
    }
}

void destroy(Stack *s)
{
    free(s->data);
    s->data = NULL;
    s->capacity = 0;
    s->top = -1;
}

void push(Stack *s, int elem)
{
    if (s->top == s->capacity - 1)
    {
        // Stack is full, resize
        s->capacity *= GROWTH_CONST;
        int *temp = realloc(s->data, s->capacity * sizeof(int));  // Temporary pointer


        if (temp == NULL)
        {
            printf("Error: Cannot reallocate memory for the stack.\n");
            free(s->data);
            exit(EXIT_FAILURE);
        }

        s->data = temp; 
    }
    s->top++;
    s->data[s->top] = elem;
}

int pop(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Error: Cannot pop from an empty stack.\n");
        free(s->data);
        exit(EXIT_FAILURE);
    }
    int topElement = s->data[s->top];
    s->top--;
    return topElement;
}

bool isEmpty(const Stack *s)
{
    return s->top == -1;
}
