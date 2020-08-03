#include <iostream>
#include "InfixToPostfix/InfixToPostfix.h"
#include "PostfixEval/PostfixEval.h"
#include "FileRW/FileRW.h"
#include <string>

using namespace std;

enum ENUM_MODES {M_FILE, M_INFIX, M_POSTFIX};
bool* parameter_processor(int argc, char** argv);

int main(int argc, char** argv) {
    // The infix/postfix expression, which will be read
    string infix;
    string postfix;
    try {
        // Working mode flags
        bool* mode = nullptr;
        mode = parameter_processor(argc, argv);
        
        // If expression is postfix
       if (mode[M_POSTFIX]) {
            // If read from file is specified
            if (mode[M_FILE]) {
                postfix = FileRead(argv[3]);
            }
            else {
                postfix = string(argv[2]);
            }
            
            // Result
            float result = postfix_eval(postfix);
            
            cout << "Result: " << result << endl;
            FileWrite("output.txt", to_string(result));
        }
       else {
           // If expresison is infix
           if (mode[M_INFIX]) {
               // If read from file is specified
               if (mode[M_FILE]) {
                   infix = FileRead(argv[3]);
               }
               else {
                   infix = string(argv[2]);
               }
           }
           // Only 1 argument is specified
           else if (argc == 2) {
               infix = string(argv[1]);
           }
           // Default/no argument
           else {
               infix = FileRead("input.txt");
           }
           
           // Convert to postfix
           postfix = infix_to_postfix(infix);
           cout << "Suffix expression: " << postfix << endl;
           
           // Result
           float result = postfix_eval(postfix);
           cout << "Result: " << result << endl;
           
           FileWrite("output.txt", postfix);
           FileWrite("output.txt", to_string(result));
        }
        delete[] mode;
    }
    catch (exception& e) {
        cout << e.what() << endl;
        string log_buffer;
        log_buffer += string("Infix expression: ") + infix;
        log_buffer += string("\nPostfix expression: ") + postfix;
        log_buffer += string("\nParameters: ");
        for (unsigned i = 1; i < argc; i++) {
            log_buffer += string(argv[i]) + ", ";
        }
        log_buffer += string("\nError: ") + e.what();
        
        FileWrite("errorlog.txt", log_buffer);
        return -1;
    }
    return 0;
}

// Function to set mode flags. Receives argc, argv from main function
bool* parameter_processor(int argc, char** argv) {
    bool* mode = nullptr;
    try {
        mode = new bool[3] {false, false, false};
    }
    catch (exception& e) {
        throw runtime_error("Error: Not sufficient memory for processing commandline parameters.");
    }
    
    // Commandline parameters. See README.md for more information.
    if (argc > 1) {
        // Set flags for modes
        for (unsigned i = 1; i < argc; i++) {
            // Begins with a '-'
            if (argv[i][0] == '-') {
                char m = argv[i][1];
                if (m == 'f') mode[M_FILE] = true;
                if (m == 'i') mode[M_INFIX] = true;
                if (m == 'p' || m == 's') mode[M_POSTFIX] = true;
            }
        }
        
        // If infix and postfix is passed at the same time
        if (mode[M_INFIX] && mode[M_POSTFIX]) {
            throw runtime_error("Parameter error: -p/-s and -i cannot be used at the same time.");
        }
        
        if (mode[M_FILE] && !mode[M_INFIX] && !mode[M_POSTFIX]) {
            throw runtime_error("Parameter error: expression type not specified.");
        }
    }
    else if (argc > 4) {
        // argc > 4 because argv[0] is the executable filename
        throw runtime_error("Parameter error: only 0 - 3 parameter(s) is accepted. Please read README.md for more information");
    }
    
    return mode;
}
