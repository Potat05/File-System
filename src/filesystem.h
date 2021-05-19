#ifndef FILESYSTEM_H
#define FILESYSTEM_H


#include <sstream>
#include <vector>


using namespace std;



/*

    We don't care about about speed here as long as it gets the job done it's good!

*/

struct File_System {
    unsigned int pointer = 0; // Bit pointer inside data
    vector<unsigned char> data; // Data as bytes

    // Saving & loading file
    bool load_file(string file_location); // Load file into data
    bool save_file(string file_location, unsigned int max_size=32*1024); // Save data into file at location

    void reset(); // Reset some stuff


    // Reading from data
    bool eof = false; // If at end of data
    bool read_bit(); // Read 1 bit
    unsigned char read_nibble(); // Read 4 bits
    unsigned char read_char(); // Read 8 bits
    unsigned short read_short(); // Read 16 bits
    string read_string(unsigned int bytes=1); // Read string

    string data_string(); // Returns data as string (Would only recommend for smaller files)

    // Writing to data
    bool inc = true; // If to increment pointer on each write
    bool ovr = false; // If data that is being written extending the data array
    void write_bit(bool val=0); // Write 1 bit
    void write_nibble(unsigned char val=0x0); // Write 4 bits
    void write_char(unsigned char val=0x00); // Write 8 bits
    void write_short(unsigned short val=0x0000); // Write 16 bits
    void write_string(string val=""); // Write string

    unsigned char pop(); // Removes last char from data and returns
    unsigned char align(); // Align the pointer to the last byte & returns amount it moved
    
    
};



#endif