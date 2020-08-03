#include <iostream>
#include "InfixToPostfix/InfixToPostfix.h"
#include "PostfixEval/PostfixEval.h"
#include "FileRW/FileRW.h"
#include <string>

using namespace std;

void printHelp();

int main(int argc, char** argv) {
    // The infix/postfix expression, which will be read
    string infix;
    string postfix;
    try {
        // Print help
        if (argc >= 2 && !strcmp(argv[1], "--help")) {
            printHelp();
            return 0;
        }
        /* Because the program retrieves 3 user input argument + 1 required argument (executable filename), 
        we will do nothing if argument number is < 4. */
        if (argc < 4) {
            throw runtime_error("Missing arguments. Please use --help for more information.");
        }

        // argv[0]: executable filename
        // argv[1]: -i / -p; or --help
        // argv[2]: input filename
        // argv[3]: ouput filename

        if (!strcmp(argv[1], "-i")) {
            // Read the expression
            infix = FileRead(argv[2]);

            // Convert and evaluate it
            postfix = infix_to_postfix(infix);
            float result = postfix_eval(postfix);

            // Print the result to stdout if nothing wrong happened
            cout << "Converted postfix expression: " << postfix << endl;
            cout << "Evaluated result: " << result << endl;

            // Write the result to file
            FileWrite(argv[3], to_string(result));
        }
        else if (!strcmp(argv[1], "-p")) {
            // Read and evaluate the postfix expression
            postfix = FileRead(argv[2]);
            float result = postfix_eval(postfix);

            // Print the result to stdout if nothing wrong happened
            cout << "Evaluated result: " << result << endl;

            // Write the result to file
            FileWrite(argv[3], to_string(result));
        }
        else {
            throw runtime_error("Invalid expression type. Please prefer --help for more information.");
        }
    }
    catch (exception& e) {
        cout << "Exception: " << e.what() << endl;

        // Write error to file
        string log_buffer;
        if (infix != "") {
            log_buffer += string("Infix expression: ") + infix + "\n";
        }
        if (postfix != "") {
            log_buffer += string("Postfix expression: ") + postfix + "\n";
        }
        
        log_buffer += string("Parameters: ");
        for (unsigned i = 1; i < argc; i++) {
            log_buffer += string(argv[i]) + ", ";
        }
        log_buffer += string("\nError: ") + e.what();

        // If the user provided enough parameters, including output filename
        if (argc == 4) {
            FileWrite(argv[3], log_buffer);
        }
        else {
            FileWrite("errorlog.txt", log_buffer);
        }
        
        return -1;
    }
    return 0;
}

void printHelp() {
    cout << "MyCalc v1.0" << endl;
    cout << "Developed by 19120615 - Hung Ngoc Phat." << endl << endl;

    cout << "Be able to handle: integers, floats, negatives." << endl;
    cout << "Accepted operators: + - * / ^" << endl << endl;

    cout << "Arguments (3): MyCalc.exe <expression type> <input filename> <output filename>" << endl;
    cout << "Where:" << endl;
    cout << "<expression type> can be either infix (-i) or postfix (-p)" << endl;
    cout << "<input filename> is the path to input file. Contains only 1 line of a proper supported expression." << endl;
    cout << "<output filename> is the path to the output file. Contains program output or exception if occurred." << endl << endl;

    cout << "Example: MyCalc.exe -i input.txt output.txt" << endl;
    cout << "Example of a valid infix expression: 34+12.4+(-3)^(1/2)" << endl;
    cout << "Example of a valid postfix expression: -2.5 3 + 1.2 ^" << endl;
}