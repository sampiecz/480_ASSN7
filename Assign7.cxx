/***********************************************************
CSCI 480 - Assignment 7 - Fall 2019
 
Progammer: Sam Piecz
Z-ID: Z1732715
Section: 2 
TA: Jingwan Li  
Date Due: Dec 02, 2019
Purpose: FAT-12 Simulation. 
************************************************************/
#include "Entry.h"
#include <iostream>
#include <iomanip>
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
using std::to_string;
using std::setw;
using std::setfill;
using std::right;

 
#define HOW_OFTEN 6
#define BLOCK_BYTES 512
#define MAX_BLOCK_ENTRIES 12
#define FAT_PRINT 240
 
// Global Vars
vector<int> FAT12(4096);
vector<Entry> entries;
string filename = "Data.txt";
int blockNumbers = 1;
 
// Prototypes
void newFile(vector<string> tokens);
void modifyFile(vector<string> tokens);
void deleteFile(vector<string> tokens);
void renameFile(vector<string> tokens);
void copyFile(vector<string> tokens);
void printStatus();

int main()
{
  cout << "Beginning of the FAT simulation." << endl;
  FAT12[0] = -1;

  int count = 0;
  string line;
  ifstream infile(filename);
  while (getline(infile, line))
  {
 
    // Every HOW_OFTEN printStatus.
    if (count % HOW_OFTEN == 0)
    {
      printStatus();
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
    if (line.find("N ") != string::npos)
    {
      cout << "Transaction: Added a new file" << endl;
      newFile(tokens);
    }
    else if (line.find("M ") != string::npos)
    {
      cout << "Transaction: Modify a file" << endl;
      modifyFile(tokens);
    }
    else if (line.find("C ") != string::npos)
    {
      cout << "Transaction: Copy a file" << endl;
      copyFile(tokens);
    }
    else if (line.find("R ") != string::npos)
    {
      cout << "Transaction: Rename a file" << endl;
      renameFile(tokens);
    }
    else if (line.find("D ") != string::npos)
    {
      cout << "Transaction: Delete a file" << endl;
      deleteFile(tokens);
    }
 
    count++;
 
  }

  cout << "\nEnd of the FAT simulation" << endl;
  printStatus();
  return 0;

}
 
// Transaction funcs
void newFile(vector<string> tokens)
{
  string addedFileName = tokens.at(1);
  string addedFileSize = tokens.at(2);
  bool here = false;
  Entry newEntry;

  for ( auto entry : entries )
  {
    if ( entry.filename == addedFileName )
    {
      here = true;
    }
  }

  if (here)
  {
    cout << "Error! The new file name, " << addedFileName << ", already exists." << endl;
  }
  else
  {
    newEntry.filename = addedFileName;
    newEntry.size = stoi(addedFileSize);
    int clusterAmount = stoi(addedFileSize) / BLOCK_BYTES;
    if ( stoi(addedFileSize) % BLOCK_BYTES > 0 )
    {
      clusterAmount++;
    }
		for( int i = 1; i <= clusterAmount; )
		{
			for( size_t j = 0; j < FAT12.size(); j++ )
			{
				if( FAT12.at(j) == 0 && i == clusterAmount )
				{
					FAT12.at(j) = -1;
					blockNumbers++;
					newEntry.clusters.push_back(j);
					i++;
					break;
				}
				else if(FAT12.at(j) == 0)
				{
					FAT12.at(j) = blockNumbers;
					blockNumbers++;
					newEntry.clusters.push_back(j);
					i++;
					break;
				}
			}
		}
    entries.push_back(newEntry);
    cout << "Successfully added a new file, " << addedFileName << " of size " << addedFileSize << endl;
  }

}

void modifyFile(vector<string> tokens)
{
  string modifiedFile = tokens.at(1);
  bool here = false;
  
  for ( auto entry : entries )
  {
    if ( entry.filename == modifiedFile )
    {
      here = true; 
    }
  }
  
  if (!here)
  {
    cout << "Error! The file name, " << modifiedFile << ", was not found." << endl;
  }
  else
  {
    newFile(tokens);
		vector<string> delVect = { "D ", modifiedFile };
    deleteFile(delVect);
		vector<string> renameVect = { "R ", modifiedFile, modifiedFile };
    renameFile(renameVect);
    cout << "Successfully modify a file, " << modifiedFile << endl;
  }
}

void deleteFile(vector<string> tokens)
{
  string deletedFile = tokens.at(1);
  bool here = false;

  for ( auto entry : entries )
  {
    if ( entry.filename == deletedFile )
    {
      here = true;
    }
  }

  if ( !here )
  {
    cout << "Error! The file name, " << deletedFile << ", was not found." << endl;
  }
  else
  {
    for ( size_t i = 0; i < entries.size(); i++ )
    {
      if ( deletedFile == entries.at(i).filename )
      {
        for ( size_t j = 0; j < entries.at(i).clusters.size(); j++ )
        {
          FAT12.at(entries.at(i).clusters.at(j)) = 0;
        }
        entries.erase(entries.begin() + i);
      }
    }
    cout << "Successfully deleted a file, " << deletedFile << endl;
  }

}

void renameFile(vector<string> tokens)
{
  string renameThisFile = tokens.at(1);
  string renamedFileName = tokens.at(2);
  bool here = false, here2 = false;

  for ( auto entry : entries )
  {
    if ( entry.filename == renameThisFile )
    {
      here = true;
    }
  }
  if (!here)
  {
    cout << "Error! The file name, " << renameThisFile << ", was not found." << endl;
  }

  for ( auto entry : entries )
  {
    if ( entry.filename == renamedFileName )
    {
      here2 = true;
    }
  }
  if ( here2 == true )
  {
    cout << "Error! The new file name, " << renamedFileName << ", already exists." << endl;
  }

  if ( here && !here2 )
  {
    for ( auto entry : entries )
    {
      if ( entry.filename == renameThisFile )
      {
        entry.filename = renamedFileName;
      }
    }
    cout << "Successfully changed the file name " << renameThisFile << " to " << renamedFileName << endl;
  }

}

void copyFile(vector<string> tokens)
{
  string copiedFile = tokens.at(1);
  string copiedFileName = tokens.at(2);

  bool here = false;
  bool leError = false;
  int size = 0;

  for ( auto entry : entries )
  {
    if ( entry.filename == copiedFile )
    {
      here = true;
      size = entry.size;
    }
  }
  if ( here == false )
  {
    cout << "Error! The old file name, " << copiedFile << ", was not found." << endl;
  }

  for ( auto entry : entries )
  {
    if ( entry.filename == copiedFileName )
    {
      leError = true;
    }
  }
  if ( leError == true )
  {
    cout << "Error! The new file name, " << copiedFileName << ", already exists." << endl;
  }

  if ( here == true && leError == false )
  {
		string addedFileName = copiedFileName;
		string addedFileSize = to_string(size);
		bool here = false;
		Entry newEntry;

		for ( auto entry : entries )
		{
			if ( entry.filename == addedFileName )
			{
				here = true;
			}
		}

		if (here)
		{
			cout << "Error! The new file name, " << addedFileName << ", already exists." << endl;
		}
		else
		{
			newEntry.filename = addedFileName;
			newEntry.size = stoi(addedFileSize);
			int clusterAmount = stoi(addedFileSize) / BLOCK_BYTES;
			if ( stoi(addedFileSize) % BLOCK_BYTES > 0 )
			{
				clusterAmount++;
			}
			for( int i = 1; i <= clusterAmount; )
			{
				for( size_t j = 0; j < FAT12.size(); j++ )
				{
					if( FAT12.at(j) == 0 && i == clusterAmount )
					{
						FAT12.at(j) = -1;
						blockNumbers++;
						newEntry.clusters.push_back(j);
						i++;
						break;
					}
					else if(FAT12.at(j) == 0)
					{
						FAT12.at(j) = blockNumbers;
						blockNumbers++;
						newEntry.clusters.push_back(j);
						i++;
						break;
					}
				}
			}
			entries.push_back(newEntry);
			cout << "Successfully added a new file, " << addedFileName << " of size " << addedFileSize << endl;
		}
    cout << "Successfully copied an existing file, " << copiedFile << ", to a new file, " << copiedFileName << endl;
  }
  
}

void printStatus()
{
  int totalFileSize = 0;
  int numFiles = 0, j = 0;
  cout << "\nDirectory Listing" << endl;
  for ( auto entry : entries )
  {
    cout << "File name: " << entry.filename << "\tFile Size: " << entry.size << endl;
    cout << "Cluster(s) in use: \t"; 
    for ( auto cluster : entry.clusters )
    {
      cout << cluster << "\t";
    }
    cout << "\n";
    totalFileSize += entry.size;
    numFiles++;
  }
  cout << "\nFiles: " << numFiles << " Total Size: " << totalFileSize << " bytes" << endl;
  cout << "\nContents of the File Allocation Table" << endl;

	for( int i =0; i < 240; i++ )
	{
		if( j % 12 == 0 )
		{
			cout << "#" << setw(3) << setfill('0') << i <<" - "<< setw(3) << setfill('0') << i+11 << ": " << "\t" << setfill(' ');
		}
		cout << right << setw(2) << FAT12.at(i) << "\t";
		j++;
		if( j % 12 == 0 )
		{
			cout << "\n";
			j = 0;
		}
	}

}
