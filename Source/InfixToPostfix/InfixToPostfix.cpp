#include "InfixToPostfix.h"

// Strip spaces from a string to process it more conveniently
void stripSpaces(string& s) {
    for (unsigned i = 0; i < s.size(); i++) {
        while (s[i] ==  ' ') {
            s.erase(i, 1);
        }
    }
}

// Get the previous character at a given position
char getPrevChar(string s, int index) {
    // If first or out of bound index, there is no previous char.
    if (index <= 0) {
        return '\0';
    }
    return s.at(index - 1);
}

// Get the next character at a given position
char getNextChar(string s, int index) {
    // If last or out of bound index, there is no next char.
    if (index >= s.size() - 1) {
        return '\0';
    }
    return s.at(index + 1);
}

/*
 Convert an infix expression to a postfix expression
 */
string infix_to_postfix(string i_expression) {
    // Strip spaces from the string to process it more conveniently
    stripSpaces(i_expression);
    
    // Init a char stack
    StackChar *stack = new StackChar;
    if (!stackInit(&stack)) {
        throw runtime_error("[Infix to Postfix] Stack init failed.");
    }
        
    string result_expr; // Postfix Expression (final result)
    unsigned index = 0;
    char c = 0;
        
    /*
     Overall algorithm
     
     Source: based on https://www.geeksforgeeks.org/stack-set-2-infix-to-postfix/
     Addition of support for float numbers, negative numbers and complex error handling/syntax tracker by me.
     
     - Loop through the infix expression.
     - If the character is a digit or a decimal point '.', add it to the postfix expr.
     - If the character is an opening parenthesis '(', push it into the stack.
     - If the character is an operator,
        + If the precedence of the current operator > precedence of stackTop() 
        (or the stack is empty, or stackTop() == '('), push the operator into the stack.
        + Else, pop the stack while the operator on the top has equal or higher precedence (priority) 
        than the currently processed operator. If a '(' is encountered, stop there. 
        Finally push the operator to the stack.
     - If the character is a closing parenthesis, pop the stack 
     (and append to the postfix expr) until an opening parenthesis is encountered (and ignore both). 
     If the stack is empty before an opening parenthesis is found, raise syntax error.
     - Else, raise syntax error.
     - Finally, when the end of the infix expression is reached, pop all elements from the stack and add them to the postfix expr.
     */
        
    // Loop through the infix expression.
    for (index = 0; index < i_expression.size(); index++) {
        // Abbreviate i_expression[index] to c
        c = i_expression[index];
        
        // If the character is a digit or '.'
        if (isdigit(c) || c == '.') {
            /* Append a space before appending the digit (to get something like '2 5 7 + * ...')
               Except when:
                    + The digit is at the beginning of the expression
                      (if we skip this, there will be a space at the beginning of the resulting postfix expression.
                   +  Before the digit was another digit or '.' or the processing number is negative 
                   (if we skip this, '1.25' will become '1 . 2 5', '-5' will become '- 5').
             */
            
            // The previous and the next character
            char prev_char = getPrevChar(i_expression, index);
            char next_char = getNextChar(i_expression, index);
            
            // Syntax tracking: before and after a '.' must be a number
            if (c == '.' && !isdigit(prev_char)) {
                string e_str = error_string_gen("Syntax error: Unexpected non-digit before '.' at ", index - 1, prev_char);
                stackFree(&stack);
                throw runtime_error(e_str);
            }
            else if (c == '.' && !isdigit(next_char)) {
                string e_str = error_string_gen("Syntax error: Unexpected non-digit after '.' at ", index + 1, next_char);
                stackFree(&stack);
                throw runtime_error(e_str);
            }
            
            // Syntax tracking: after a digit must be either a '.', an operator, another digit, a ')', or a '\0'
            if (isdigit(c) && next_char != '\0' && 
                next_char != '.' && !isoperator(next_char) && 
                !isdigit(next_char) && next_char != ')') {
                string e_str = error_string_gen("Syntax error: Unexpected token after digit at", index + 1, next_char);
                stackFree(&stack);
                throw runtime_error(e_str);
            }

            // Check if the current number is negative or not
            char prev_prev_char = getPrevChar(i_expression, index - 1);
            bool isNegative = (prev_char == '-' && !isdigit(prev_prev_char) && prev_prev_char != ')');
            /* If a number is negative, the char before it must be a '-', and the char before the '-' must not be a digit or a ')'
             Example:
             2-5 => 5 is not negative
             -5 => -5 is negative. prev_prev_char is '\0', so it is not a number.
             (-5 => -5 is negative. prev_prev_char is '(', so it is not a number.
             (2+4)-5 => 5 is not negative because prev_prev_char is ')'.
             2*-5 => -5 is negative
             ....
            */
            
            // If previous char is not a digit, a dot and the current char is not negative
            if (index != 0 && !isdigit(prev_char) && prev_char != '.' && !isNegative)
                result_expr += ' ';
            
            // Finally append the digit/dot
            result_expr += c;
        }
        
        // Ignore the space
        else if (c == ' ' ) {
            continue;
        }
        
        // If the char is an open parenthesis
        else if (c == '(') {
            // Push it into the stack
            stackPush(stack, c);
        }
        
        // If the char is an operator
        else if (isoperator(c)) {
            char prev_char = getPrevChar(i_expression, index);
            char next_char = getNextChar(i_expression, index);
            
            /* Negative number handling: if the operator is a '-', and it is followed immediately by a digit, 
            and the previous char must not be a number (if so, it is a minus operator, not a negative sign), 
            then append it into the result expression to form a negative number. 
            Skip processing the '-' and go on to the next char.
            */
            if (c == '-' && isdigit(i_expression[index + 1]) && !isdigit(prev_char)) {
                result_expr += ' ';
                result_expr += c;
                continue;
            }
            
            // Syntax tracker: before an operator must be a digit or a ')'.
            if (!isdigit(prev_char) && prev_char != ')') {
                string e_str = error_string_gen("Conversion syntax error: unexpected non-operand before an operator at ", index, prev_char);
                stackFree(&stack);
                throw runtime_error(e_str);
            }
            // Syntax tracker: after an operator mustn't be another operator
            if (isoperator(next_char)) {
                string e_str = error_string_gen("Conversion syntax error: unexpected operator after an operator at ", index + 1, next_char);
                stackFree(&stack);
                throw runtime_error(e_str);
            }
            
            // Common steps for other cases
            // If the stack is empty, or precedence of c > precedence of stack top
            if (stackIsEmpty(stack) || priority(c) > priority(stackTop(stack))) {
                stackPush(stack, c);
            }
            else {
                // Pop and append while precedence of top > precedence of c
                while (!stackIsEmpty(stack) && priority(c) <= priority(stackTop(stack))) {
                    result_expr += ' ';
                    result_expr += stackPop(stack);
                }
                stackPush(stack, c);
            }
        }
        
        // If a closing parenthesis is encountered
        else if (c == ')') {
            try {
                while (stackTop(stack) != '(') {
                    result_expr += ' ';
                    result_expr += stackPop(stack);
                }
                // Finally pop and ignore the '('
                stackPop(stack);
            }
            // While popping, if the stack is empty, raise syntax error
            catch (exception& e) {
                if (strcmp(e.what(), ERR_EMPTY_STACK) == 0) {
                    string e_str = error_string_gen("Conversion syntax error: no matching opening parenthesis with closing at ", index, i_expression[index]);
                    stackFree(&stack);
                    throw runtime_error(e_str);
                }
            }
        }
        
        // Else, raise syntax error
        else {
            string e_str = error_string_gen("Conversion syntax error: Invalid character at ", index, i_expression[index]);
            stackFree(&stack);
            throw runtime_error(e_str);
        }
    }
    
    // Pop all remaining operators from the stack
    while (!stackIsEmpty(stack)) {
        result_expr += ' ';
        result_expr += stackPop(stack);
    }
    
    stackFree(&stack);
    return result_expr;
}
