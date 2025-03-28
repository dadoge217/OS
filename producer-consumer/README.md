# Producer/Consumer
# Description
This C++ project is a solution to the producer/consumer problem. The producer generates random numbers between 1-100 and places them in a text files which acts as shared memory and can only hold two items at a time. When shared memory is full, producer signals to the consumer procedure using two semaphores. After the consumer consumes these values, it flips the semaphore and returns control of the critical section to the producer. This cycle repeats until the end of a defined loop (set to 10 cycles).
# Installation & Execution
This folder contains all the files needed.
The files used during execution are:
```
producer.cpp
consumer.cpp
header.hpp
shared.txt
```
# Dependencies
The following libraries are used in this program:
```
<stdio.h>
<stdlib.h>
<string.h>
<fcntl.h>
<sys/shm.h>
<sys/stat.h>
<sys/mman.h>
<unistd.h>
<semaphore.h>
<pthread.h>
<iostream>
<random>
<cstring>
```
Hardware Versions used:  
Visual Studio Code v1.98.2  
OS: Windows_NT x64 10.0.26100  
# Compilation
Steps before compilation:  
  1. Load Visual Studio Code in a Linux/UNIX environment  
  2. Use the terminal and enter the datapath to the folder

The consumer and producer are separate files. To compile, run:
```
$ g++ producer.cpp -pthread -lrt -o producer
$ g++ consumer.cpp -pthread -lrt -o consumer
$ ./producer & ./consumer &
```
# Example
https://github.com/user-attachments/assets/4d7070d4-154e-496a-8b10-eb28409dfe92
Example 1: Producer is run in the background; consumer is run in the foreground

https://github.com/user-attachments/assets/fbb50aea-e7c3-49d3-b9e8-d9763de08287
Example 2: Producer and consumer are both run in the background

