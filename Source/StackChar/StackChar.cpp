#include "StackChar.h"

/*
 Initializes the stack
 Returns true if allocated successfully
 Otherwise, returns false
 */
bool stackInit(StackChar **w_stack) {
    try {
        *w_stack = new StackChar;
    }
    catch (exception& e) {
        return false;
    }
    return true;
}

/*
 Frees the stack
*/
void stackFree(StackChar **w_stack) {
    if (*w_stack == nullptr) return;
    
    for (SNodeChar* p = (*w_stack)->top; p != nullptr; ) {
        SNodeChar* down_node = p->down;
        delete p;
        p = down_node;
    }
    
    delete *w_stack;
    *w_stack = nullptr;
}

/*
 Checks if the stack is empty
*/
bool stackIsEmpty(const StackChar *r_stack) {
    return (r_stack->top == nullptr);
}

/*
 Deletes the top element of the stack and returns its value (pop and top at the same time, just like in Assembly language)
*/
char stackPop(StackChar *u_stack) {
    if (u_stack == nullptr) {
        throw runtime_error(ERR_NULL_STACK);
    }
    if (stackIsEmpty(u_stack)) {
        throw runtime_error(ERR_EMPTY_STACK);
    }
    
    float pop_val = u_stack->top->val;
    SNodeChar* down_node = u_stack->top->down;
    delete u_stack->top;
    u_stack->top = down_node;
    
    return pop_val;
}

/*
 Adds an element to the top of the stack
*/
void stackPush(StackChar *u_stack, char r_val) {
    if (u_stack == nullptr) {
        throw runtime_error(ERR_NULL_STACK);
    }
    
    SNodeChar* new_node = new SNodeChar {r_val, u_stack->top};
    u_stack->top = new_node;
}

/*
 Read the element at the top of the stack
*/
char stackTop(StackChar *r_stack) {
    if (r_stack == nullptr) {
        throw runtime_error(ERR_NULL_STACK);
    }
    if (stackIsEmpty(r_stack)) {
        throw runtime_error(ERR_EMPTY_STACK);
    }
    
    return r_stack->top->val;
}

/*
 Print all the elements of the stack from top to bottom
*/
void stackPrint(StackChar *r_stack) {
    if (r_stack == nullptr) {
        cout << ERR_NULL_STACK << endl;
        return;
    }
    if (stackIsEmpty(r_stack)) {
        cout << ERR_EMPTY_STACK << endl;
        return;
    }
    
    for (SNodeChar* p = r_stack->top; p != nullptr; p = p->down) {
        cout << p->val << " " << endl;
    }
    cout << endl;
}
