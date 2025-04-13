#include <iostream>
#include "Stack.h"

void testPush(Stack &s, int elem)
{
    s.push(elem);
    std::cout << "Top element after push: " << elem << '\n';
}

void testPop(Stack &s)
{
    if (!s.isEmpty())
    {
        int popped = s.pop();
        std::cout << "Popped: " << popped << std::endl;
    }
    else
    {
        std::cout << "Stack is empty, cannot pop.\n";
    }
}

int main()
{
    Stack s;

    std::cout << "=== Test 1: Pushing elements up to initial capacity ===\n";
    testPush(s, 1);
    testPush(s, 2);
    testPush(s, 3);
    testPush(s, 4);

    std::cout << "=== Test 2: Growing stack beyond initial capacity ===\n";

    testPush(s, 5);
    testPush(s, 6);

    std::cout << "=== Test 3: Popping elements ===\n";

    testPop(s);
    testPop(s);
    testPop(s);
    testPop(s);
    testPop(s);
    testPop(s);

    std::cout << "=== Test 4: Attempt to pop from an empty stack ===\n";
    testPop(s);

    std::cout << "=== Test 5: Pushing 100 elements ===\n";
    for (int i = 0; i < 100; i++)
    {
        testPush(s, i);
    }

    std::cout << "=== Test 6: Popping 101 elements ===\n";
    for (int i = 0; i < 101; i++)
    {
        testPop(s);
    }


    return 0;
}