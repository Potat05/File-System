https://github.com/Potat05/File-System

```c++

#include "file system/src/filesystem.h" // Use the filesystem

#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 2) return 0;

    // arg[1] is file dragged onto exe

    // Load file
    File_System fs;
    fs.load_file(argv[1]);

    // Get data as string
    cout << fs.data_string() << endl;

    fs.reset() // Resets pointer & eof & ovr
    // Pointer is bit in file to read/write
    // eof is if the pointer is at end of file
    // ovr is if writing added more data to the file
    
    fs.read_bit(); // Read bit from pointer
    fs.read_char(); // Read char from pointer
    fs.read_string(10); // Read string from pointer
    
    fs.reset();

    // Increment each byte by 1
    while(!fs.ovr) { // Keep looping until we write extra bytes
        char val = fs.read_byte();
        fs.pointer -= 8; // Move pointer so we can write to the byte we just read from
        fs.write_byte(val + 1);
    }
    fs.pop(); // Remove last extra byte that was added

    cout << fs.data_string() << endl;






    while(True) {} // Pause at end so we can see output
}

```