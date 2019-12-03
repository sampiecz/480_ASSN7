/***********************************************************
 CSCI 480 - Assignment 7 - Fall 2019
 
 Progammer: Sam Piecz
 Z-ID: Z1732715
 Section: 2 
 TA: Jingwan Li  
 Date Due: Dec 02, 2019 
 Purpose: FAT-12. 
 ************************************************************/
#ifndef ENTRY_H
#define ENTRY_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Entry {
  public:
    Entry();
    int size;
    string filename;
    int startingAddress;
    vector<int> clusters;
};

#endif
