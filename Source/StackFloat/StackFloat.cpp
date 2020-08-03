#include "StackFloat.h"

/*
 Initializes the stack
 Returns true if allocated successfully
 Otherwise, returns false
 */
bool stackInit(StackFloat **w_stack) {
    try {
        *w_stack = new StackFloat;
    }
    catch (exception& e) {
        return false;
    }
    return true;
}

/*
 Frees the stack
*/
void stackFree(StackFloat **w_stack) {
    if (*w_stack == nullptr) return;
    
    for (SNodeFloat* p = (*w_stack)->top; p != nullptr; ) {
        SNodeFloat* down_node = p->down;
        delete p;
        p = down_node;
    }
    
    delete *w_stack;
    *w_stack = nullptr;
}

/*
 Checks if the stack is empty
*/
bool stackIsEmpty(const StackFloat *r_stack) {
    return (r_stack->top == nullptr);
}

/*
 Deletes the top element of the stack and returns its value (pop and top at the same time, just like in Assembly language)
*/
float stackPop(StackFloat *u_stack) {
    if (u_stack == nullptr) {
        throw runtime_error(ERR_NULL_STACK);
    }
    if (stackIsEmpty(u_stack)) {
        throw runtime_error(ERR_EMPTY_STACK);
    }
    
    float pop_val = u_stack->top->val;
    SNodeFloat* down_node = u_stack->top->down;
    delete u_stack->top;
    u_stack->top = down_node;
    
    return pop_val;
}

/*
 Adds an element to the top of the stack
*/
void stackPush(StackFloat *u_stack, float r_val) {
    if (u_stack == nullptr) {
        throw runtime_error(ERR_NULL_STACK);
    }
    
    SNodeFloat* new_node = new SNodeFloat {r_val, u_stack->top};
    u_stack->top = new_node;
}

/*
 Read the element at the top of the stack
*/
float stackTop(StackFloat *r_stack) {
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
void stackPrint(StackFloat *r_stack) {
    if (r_stack == nullptr) {
        cout << ERR_NULL_STACK << endl;
        return;
    }
    if (stackIsEmpty(r_stack)) {
        cout << ERR_EMPTY_STACK << endl;
        return;
    }
    
    for (SNodeFloat* p = r_stack->top; p != nullptr; p = p->down) {
        cout << p->val << " " << endl;
    }
    cout << endl;
}
