/***********************************************************
CSCI 480 - Assignment 6 - Fall 2019
 
Progammer: Sam Piecz
Z-ID: Z1732715
Section: 2 
TA: Jingwan Li  
Date Due: Nov 13, 2019 Purpose: Memory Management. 
************************************************************/
#include "Block.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <list>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <vector>
 
using std::istringstream;
using std::istream_iterator;
using std::ifstream;
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::list;
 
#define HOW_OFTEN 5
 
// Global Vars
string filename = "Data.txt";
 
// Prototypes
void printStatus();
 
int main()
{
  cout << "Beginning of the FAT simulation." << endl;
 
 
  int id, size, processId; 
  string line, blockId;
  ifstream infile(filename);
  while (getline(infile, line))
  {
 
    // Every HOW_OFTEN printStatus.
    if (count % HOW_OFTEN == 0)
    {
      printStatus(false);
    }
 
    // Check for delimiter
    if (line.find("?") != string::npos)
    {
      break;
    }
 
    // Break line by spaces 
    istringstream iss(line);
    vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
 
    // See what type of transaction and handle appropriately
    if (line.find("L") != string::npos)
    {
    }
    else if (line.find("A") != string::npos)
    {
    }
    else if (line.find("D") != string::npos)
    {
    }
    else if (line.find("T") != string::npos)
    {
    }
 
    count++;
 
  }
 
  // 6. Display exit message. 
    cout << "\nEnd of the run" << endl;
 
  // 4. Print empty lists.
  printStatus(false);
  return 0;
}
 
// Function Definitions
void printStatus()
{
}
