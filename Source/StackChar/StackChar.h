#ifndef Stack_Char_h
#define Stack_Char_h

#include <iostream>
#include <string>
using namespace std;

// Error codes
#define ERR_NULL_STACK "Error: Null stack"
#define ERR_EMPTY_STACK "Error: Empty stack"

struct SNodeChar {
    char val = 0;
    SNodeChar *down = nullptr;
};

struct StackChar {
    SNodeChar *top = nullptr;
};

bool stackInit(StackChar **w_stack);

void stackFree(StackChar **w_stack);

bool stackIsEmpty(const StackChar *r_stack);

char stackPop(StackChar *u_stack);

void stackPush(StackChar *u_stack, char r_val);

char stackTop(StackChar *r_stack);

void stackPrint(StackChar *r_stack);
#endif /* Stack_h */
