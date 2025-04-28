// Colin Grant
// banker.hpp
// 4/24/25

#ifndef BANKER_HPP
#define BANKER_HPP

#include <iostream>
#include <fstream>

#define NUMRESOURCES 3 // The number of different resources
#define NUMPROCESSES 5 // The number of different processes      
#define ENDLINE ';' // The end of a list type in the file
#define ENDLIST ',' // The end of a value in the file

void processTables(char* file, int allocated[][NUMRESOURCES], int max[][NUMRESOURCES], int* available);
int main(int argc, char* argv[]);

#endif
