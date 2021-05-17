
#include <windows.h>
#include <iostream>

#include "filesystem.h"

using namespace std;




int main(int argc, char *argv[]) {

    if(argc != 2) {
        cout << "No file loaded!" << endl;
        while(true) Sleep(1);
    }



    File_System fs;
    fs.load_file(argv[1]);

    // Print whole file
    cout << fs.data_string() << endl;

    // Print whole file with 1 bit offset
    fs.pointer = 1;
    cout << fs.read_string(9999) << endl;

    // Increment each character by 1
    fs.reset();
    while(!fs.eof) {
        char val = fs.read_char();
        fs.pointer -= 8;
        fs.write_char(val + 1);
    }
    cout << fs.data_string() << endl;
    




    while(true) Sleep(1);
}