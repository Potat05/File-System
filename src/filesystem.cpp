#include "filesystem.h"

#include <sstream>
#include <fstream>
#include <vector>

#include <iostream>


using namespace std;



bool File_System::load_file(string file_location) {

    ifstream file(file_location); // Load file
    
    if(!file.good() || !file.is_open()) return false; // If file not found or failed to open
    
    // Get file size
    file.seekg(0, ios::end);
    int length = file.tellg();
    file.seekg(0, ios::beg);

    if(length <= 0) return false; // Error loading file

    // Load file into data
    data.reserve(length);
    copy(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), back_inserter(data));
    reset();

    return true;
}

bool File_System::save_file(string file_location, unsigned int max_size) {
    if(data.size() > max_size) return false; // Just incase if there's a error so we don't save a massive file

    // Open file
    ofstream file;
    file.open(file_location);

    if(!file.is_open() || !file) return false; // If file fails to open

    // Save data onto file
    string str = "";
    for(unsigned int i=0; i < data.size(); i++) str += data[i];
    file << str;
    file.close();

    return true;
}



void File_System::reset() {
    pointer = 0;
    eof = false;
    ovr = false;   
}







bool File_System::read_bit() {
    if(eof) return 0; // If end of file don't read junk bytes
    if(pointer+1 >= data.size()*8) eof = true; // Set end of file to true if at end of file

    // Read bit
    bool val = (data[(pointer / 8)] >> (pointer % 8)) & 0x01;
    pointer++;
    return val;
    
}

unsigned char File_System::read_nibble() {
    if(eof) return 0; // If end of file don't read junk bytes

    // Read each bit 1 by 1
    unsigned char val = 0x0;

    for(unsigned char i=0; i < 4; i++) {
        if(read_bit()) { // If bit is set set bit in return byte
            val |= (char)(1 << (3 - i));
        }
    }

    return val;
}

unsigned char File_System::read_char() {
    if(eof) return 0; // If end of file don't read junk bytes

    if(pointer % 8 == 0) { // Fast read, we can just read the byte if pointer is byte aligned
        if(pointer+8 >= data.size()*8) eof = true;
        unsigned char val = data[(pointer) / 8];
        pointer += 8;
        return val;
    }

    // Slow read, read each bit 1 by 1
    unsigned char val = 0x00;

    for(unsigned char i=0; i < 8; i++) {
        if(read_bit()) { // If bit is set set bit in return byte
            val |= (char)(1 << (7 - i));
        }
    }

    return val;
}

unsigned short File_System::read_short() {
    if(eof) return 0;

    // Read 2 bytes and combine to short
    return ((unsigned short)read_char() << 8) | (unsigned short)read_char();
}

string File_System::read_string(unsigned int bytes) {
    string val = "";
    for(unsigned int i=0; i < bytes && !eof; i++) {
        val += read_char();
    }
    return val;
}




string File_System::data_string() {
    string str = "";
    for(unsigned int i=0; i < data.size(); i++) {
        str += data[i];
    }
    return str;
}







void File_System::write_bit(bool val) {
    unsigned int ind = pointer / 8;
    unsigned char shift = pointer % 8;

    if(ind >= data.size()) { // Add another byte to the end of the data if it's at the data limit
        data.push_back(0x00);
        ovr = true; // And update this flag accordingly
    }

    if(inc) pointer++; // Increment pointer if inc flag is set

    // Toggle bit if bit doesn't equal value
    if((bool)((data[ind] >> shift) & 0x01) != val) data[ind] ^= 0x01 << shift;
}

void File_System::write_nibble(unsigned char val) {
    for(unsigned char i=0; i < 4; i++) { // We can just write 4 bits
        write_bit(((val & (1 << i)) != 0));
    }
}

void File_System::write_char(unsigned char val) {
    if(pointer % 8 == 0) { // Fast write, we can just set the byte if pointer is byte aligned
        if(pointer >= data.size()*8) { // Add the val to the end of the data if it's at the data limit
            data.push_back(val);
            ovr = true; // And update this flag accordingly
        } else {
            data[pointer / 8] = val;
        }
        if(inc) pointer += 8;
        return;
    }
    
    for(unsigned char i=0; i < 8; i++) { // We can just write 8 bits
        write_bit(((val & (1 << i)) != 0));
    }
}

void File_System::write_short(unsigned short val) {
    write_char(val); // Just split the value into 2 bytes then write those
    write_char(val >> 8);
}

void File_System::write_string(string val) {
    for(unsigned int i=0; i < val.length(); i++) {
        write_char(val[i]);
    }
}



unsigned char File_System::pop() {
    unsigned char val = data[data.size()-1];
    data.pop_back();
    return val;
}

unsigned char File_System::align() {
    unsigned char dist = pointer % 8;
    pointer -= dist;
    return dist;
}