/*****************************************************************************/
/*                                                                           */
/*  Filename:     indextest.cpp                                           */
/*  Author(s):    Jim Skon                                                   */
/*  Last Update:  2/10/2013                                                  */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <list>
// Include stemming stuff
#include "english_stem.h"

using namespace std;

#include "textindex.h"


/*****************************************************************************/
/*                                                                           */
/*      Main program - command line arguments:                               */
/*            Search term                                                    */
/*                                                                           */
/*****************************************************************************/
main () {

  list<int> result;
  list<int>::iterator it;
  int position; 
  string fileName = "Shakespeare.txt";
  ifstream textFile;
  string line;  // A line from the text file
  stemming::english_stem<char, std::char_traits<char> > StemEnglish;

  // Word is the word we are looking for
  string word;  
  
  /* Build an index of the text */
  buildTextIndex (fileName);

  // Open the text file
  textFile.open(fileName.c_str(), ios::in);
  if (textFile.bad()) {
    cout << "File: " << fileName << " cannot be opened" << endl;
    exit (1);
  }

  while (1){
    cout << "Enter Word to search for: ";
    cin >> word;

    /* get the stem of the search word */
    StemEnglish(word);
    cout << "find: " << word << endl;
    
    // search for all lines with this word
    result = indexSearch(word);
    
    // Give the match count
    cout << "Number of matches:" << result.size() << endl;
    
    
    /* Traverse the list of references */
    for (it = result.begin() ; it != result.end(); it++)
      {
	// Grab the postion of the reference out of the iterator
	position = (*it);
	// Seek that line in the file, and display it
	textFile.seekg (position, ios::beg);
	getline(textFile,line);
	cout << line << endl;
      }
    cout << endl;
  }  
}
  
