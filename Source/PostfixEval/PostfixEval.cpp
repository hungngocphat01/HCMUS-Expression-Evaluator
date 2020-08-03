#include "PostfixEval.h"

// Error messages
#define INVALID_OPERAND "Syntax error: Invalid operand at "
#define INVALID_OPERATOR "Syntax error: Invalid operator at "
#define OTHER_EXCEPTION "Other exception at "

/*
 Evaluate a postfix (suffix) expression given in string type.
 - p_expression: a postfix expression to be evaluated. Operands/operators MUST be seperated by spaces
 - Returns the result.
 */
float postfix_eval(string p_expression) {
    // Add a space after the expression in case there is only one number in it
    p_expression += ' ';
    
    float result = 0;
    
    StackFloat* stack = nullptr;
    
    unsigned index = 0; // Index variable to loop through the expression
    char c = 0;         // Character at current position (abbreviation)
    
    // If init fails, return false
    if(!stackInit(&stack)) {
        throw runtime_error("[Postfix] Stack allocation failed.");
    }
    
    string proc_operand = ""; // The operand that is being processed
    
    /* Overall algorithm:
     
     Based on lecture 'Hàng đợi' on 24/6/2020 by Lecturer N.T.Việt, fit@HCMUS.
     Addition of support for float number, negative number and error handling/syntax tracker by me.
     
     - Iterate through the expression string.
     - If the char is a digit or a decimal point '.' (operand), add it into the proc_operand string.
     - If the char is a space and proc_operand is not empty (which means there was a number before that space), convert to float and push the proc_operand into the stack, and set proc_operand to empty string. Else (before the space was an operator), ignore that space.
     - If the char is an operator,
        + If there was a number before the operator (which means proc_operand != ""), syntax error.
        + Else, pop 2 elements from the stack, perform evaluation, then push the result into the stack again.
     - Else, syntax error.
    */
    
    try {
        // Loop through the expression string
        for (index = 0; index < p_expression.size(); index++) {
            // Abbreviate p_expression[index] to c
            c = p_expression[index];
            
            // If c is part of an operand (a number)
            if (isdigit(c) || c == '.') {
                proc_operand += c;
            }
            // If c is a space
            else if (c == ' ') {
                // If there was no number before it (but maybe operator), ignore this space
                if (proc_operand == "") continue;
                
                // Else, push the number into stack
                stackPush(stack, std::stof(proc_operand)); // stof = string to float
                proc_operand = "";
            }
            // If c is an operator
            else if (isoperator(c)) {
                // If the operator is a '-', and it is followed by a digit, then append it into the proc_operand. Skip processing the '-' and go on to the next char.
                if (c == '-' && isdigit(p_expression[index + 1])) {
                    proc_operand += c;
                    continue;
                }
                
                // But there is no space before it -> syntax error
                if (proc_operand != "") {
                    string err = error_string_gen(INVALID_OPERATOR, index, p_expression[index], "May be a space is missing.");
                    // string err = string("Syntax error: Invalid operator: '") + c + string("' at ") + to_string(index) + string(". May be a space is missing.");
                    stackFree(&stack);
                    throw runtime_error(err);
                    // For example: Syntax error: Invalid operator: '+' at xxx ....
                    break;
                }
                // Else if everything is ok
                else {
                    // Result of popping and evaluating 2 top elements
                    float sub_result = 0;

                    // Pop 2 elements and check for error
                    float operand1 = stackPop(stack);
                    float operand2 = stackPop(stack);
                    
                    // Evaluate
                    switch (c) {
                        case '+':
                            sub_result = operand2 + operand1;
                            break;
                        case '-':
                            sub_result = operand2 - operand1;
                            break;
                        case '*':
                            sub_result = operand2 * operand1;
                            break;
                        case '/':
                            sub_result = operand2 / operand1;
                            break;
                        case '^':
                            sub_result = pow(operand2, operand1);
                            break;
                        // 'default' isn't needed because the isoperator() function checked this before.
                    }
                    
                    // Pushes the result into the stack
                    stackPush(stack, sub_result);
                }
            }
            // Other syntax errors
            else {
                string err = error_string_gen(INVALID_OPERAND, index, p_expression[index]);
                stackFree(&stack);
                throw runtime_error(err);
            }
        }
    }
    catch (const exception& e) {
        // If the stack is empty while processing, it might be due to an operand is missing
        // For example: 5 + +
        if (strcmp(e.what(), ERR_EMPTY_STACK) == 0) {
            string err = error_string_gen(INVALID_OPERATOR, index, p_expression[index], "May be an operand is missing.");
            stackFree(&stack);
            throw runtime_error(err);
        }
        
        // If other exception (like null stack)
        string err = error_string_gen(OTHER_EXCEPTION, index, p_expression[index], e.what());
        stackFree(&stack);
        throw runtime_error(err);
    }
    
    // The result is what remained in the stack
    result = stack->top->val;
    
    stackFree(&stack);
    return result;
}
