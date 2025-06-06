# Banker's Algorithm
# Description
This C++ project is an implementation of the banker's algorithm. The table initializing function reads in data from lines in an input file to create various tables (max, allocated, available). Then, the main function loops through the customer processes repeatedly. If the bank (max) has enoughs resources to meet the process's request, it allocates those resources, marks the customer as finished, and takes the resources back to reallocate. This repeats until all customers are satisfied or if there are not enough resources to meet a demand. Main will then print out the order and whether or not the processes are in a safe state.
# Installation & Execution
This folder contains all the files needed.
The files used during execution are:
```
banker.cpp
banker.hpp
tables.txt
```
# Dependencies
The following libraries are used in this program:
```
<iostream>
<fstream>
```
Hardware Versions used:  
Visual Studio Code v1.99.3
OS: Windows_NT x64 10.0.26100  
# Compilation
Steps before compilation:  
  1. Load Visual Studio Code in a Linux/UNIX environment  
  2. Use the terminal and enter the datapath to the folder

The banker and text files are separate. To compile, run:
```
$ g++ -o banker banker.cpp
$ ./banker tables.txt
```
# Example
https://github.com/user-attachments/assets/074948ec-dc89-451a-b3bb-205a2073c4a0
Example 1: Bank processes the requested setup of resources and processes.

https://github.com/user-attachments/assets/1f57da8f-d7fc-4a96-9207-c16f9e9250cb
Example 2: Bank does not have enough resources to meet customer request.

