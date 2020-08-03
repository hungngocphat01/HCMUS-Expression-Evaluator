#ifndef Utils_h
#define Utils_h

#include <string>
using std::string;
using std::to_string;

// This file contains several shared functions used in PostfixEval and InfixToPostfix
// Because these functions are shared amongs different files (included twice), they have to be 'inline'.
// Without 'inline', symbol duplication is likely to happen, which leads to compilation failure.

/*
 Checks if a character is an operator
*/
static inline bool isoperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

/*
 Returns the precedence (priority) of an operator
*/
static inline unsigned priority(char c) {
    if (isoperator(c)) {
        switch (c) {
            case '^':
                return 3;
                break;
            case '*':
            case '/':
                return 2;
            case '+':
            case '-':
                return 1;
            // No 'default' because isOperator() checked that case in the first place.
        }
    }
    // Brackets, etc.
    return 0;
}

/* Generate a syntax error description
    err_noti: error notification string/type of the error.
    position: where the error occured. Hide the position when -1 is passed.
    err_char: the character causing the error
    addt: Additional info (optional)
 
    Example: "Syntax error at 3: 'a'. Invalid operand."
 */
static inline string error_string_gen(string err_noti, int position, char err_char, string addt = "") {
    string e_str;
    e_str += err_noti;
    if (position >= 0) {
        e_str += to_string(position);
    }
    e_str += ": '";
    e_str += err_char;
    e_str += "'. ";
    e_str += addt;
    return e_str;
}

#endif /* Utils_h */
