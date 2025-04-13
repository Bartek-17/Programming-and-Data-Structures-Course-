#include <stdio.h>
#include "Stack.h"

void testPush(Stack *s, int elem)
{
    printf("Pushing %d...\n", elem);
    push(s, elem);
    printf("Top element after push: %d\n", s->data[s->top]);
}

void testPop(Stack *s)
{
    if (!isEmpty(s))
    {
        int popped = pop(s);
        printf("Popped: %d\n", popped);
    }
    else
    {
        printf("Stack is empty, cannot pop.\n");
    }
}

int main()
{
    Stack s;
    init(&s);

    printf("=== Test Case 1: Pushing Elements up to initial capacity===\n");
    testPush(&s, 1);
    testPush(&s, 2);
    testPush(&s, 3);
    testPush(&s, 4);

    printf("\n=== Test Case 2: Stack Growth beyond initial capacity ===\n");
    testPush(&s, 5);
    testPush(&s, 6);

    printf("\n=== Test Case 3: Popping Elements ===\n");
    testPop(&s);
    testPop(&s);
    testPop(&s);
    testPop(&s);
    testPop(&s);
    testPop(&s);

    printf("\n=== Test Case 4: Attempt to pop from Empty Stack ===\n");
    testPop(&s);

    destroy(&s);

    

    init(&s);

    printf("\n=== Test Case 5: Pushing 100 elements ===\n");
    for (int i = 0; i < 100; i++)
    {
        testPush(&s, i);
    }

    printf("\n=== Test Case 6: Popping 101 elements ===\n");
    for (int i = 0; i < 101; i++)
    {
        testPop(&s);
    }

    pop(&s);
    destroy(&s);

    

    return 0;
}
