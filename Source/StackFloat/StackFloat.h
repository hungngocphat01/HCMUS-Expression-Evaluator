#ifndef Stack_Float_h
#define Stack_Float_h

#include <iostream>
#include <string>
using namespace std;

// Error codes
#define ERR_NULL_STACK "Error: Null stack"
#define ERR_EMPTY_STACK "Error: Empty stack"

struct SNodeFloat {
    float val = 0;
    SNodeFloat *down = nullptr;
};

struct StackFloat {
    SNodeFloat *top = nullptr;
};

bool stackInit(StackFloat **w_stack);

void stackFree(StackFloat **w_stack);

bool stackIsEmpty(const StackFloat *r_stack);

float stackPop(StackFloat *u_stack);

void stackPush(StackFloat *u_stack, float r_val);

float stackTop(StackFloat *r_stack);

void stackPrint(StackFloat *r_stack);
#endif /* Stack_h */
