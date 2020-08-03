#ifndef FileRW_h
#define FileRW_h

#include <string>
#include <cstring>
#include <stdexcept>
#include <iostream>
using std::string;
using std::exception;
using std::runtime_error;
using std::cout;
using std::endl;

string FileRead(const char* filename);
void FileWrite(const char* filename, string content);

#endif /* FileRW_h */
