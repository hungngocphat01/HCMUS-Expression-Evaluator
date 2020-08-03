#include "FileRW.h"

string FileRead(const char* r_filename) {
    // The buffer to be read
    char* buffer = new char[200];
    
    // Init the file pointer
    FILE* fp = nullptr;
    fp = fopen(r_filename, "rt");
    
    if (fp == nullptr) {
        fclose(fp);
        throw runtime_error("Error reading file: input file does not exist.");
    }
    
    // The pointer holding the address of the read buffer for later check
    char* read_chars = fgets(buffer, 199, fp);
    // Close the file as we don't need it anymore
    fclose(fp);
    
    // If error occurred while reading
    if (read_chars == nullptr) {
        throw runtime_error("Error reading file: file opened but was not properly read.");
    }
    
    // Remove the \n at the end of the buffer
    for (unsigned i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
    }
        
    // Convert char* to string and free the dynamic char array
    string content_str = string(read_chars);
    delete[] buffer;

    return content_str;
}

void FileWrite(const char* filename, string content) {
    // Check if the content to be written is not empty
    if (content.size() == 0) {
        throw runtime_error("Error writing file: the buffer is empty.");
    }
    
    // Initialize a file pointer
    FILE* fp = nullptr;
    fp = fopen(filename, "wt");
    
    // Check if the file has been successfully opened/created
    if (fp == nullptr) {
        throw runtime_error("Error writing file: cannot open or create new file for writing.");
    }
    
    // Write to file and get the number of chars written
    int written_chars_num = fprintf(fp, "%s", content.c_str());
    
    // If write failed
    if (written_chars_num == EOF && ferror(fp)) {
        fclose(fp);
        throw runtime_error("Error writing file: cannot write to file.");
    }
    
    // If write succeeded but not enough chars
    if (written_chars_num != content.size()) {
        cout << endl << "Warning: file written but not enough chars." << endl;
        cout << "Wrote " << written_chars_num << " chars instead of " << content.size() << endl;
    }
    fclose(fp);
}
