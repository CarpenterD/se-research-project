#ifndef FILEIO
#define FILEIO

#include <iostream>
#include <fstream>
#include <string>

// Creates an ifstream to a file and outputs a pointer to it
// Fails if file could not be opened
std::istream* CreateInputStream(std::string fileName);

/* Creates (allocates) an ofstream to a file and returns a pointer to it.
 * Truncates target file if one is found.
 * Uses stdout if no name is given. */
std::ostream* CreateOutputStream(std::string fileName);

#endif