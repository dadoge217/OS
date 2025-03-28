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
# Compilation
Steps before compilation:
```
1. Load Visual Studio Code in a Linux/UNIX environment
2. Use the terminal and enter the datapath to the folder
```
The consumer and producer are separate files. To compile, run:
```
$ g++ producer.cpp -pthread -lrt -o producer
$ g++ consumer.cpp -pthread -lrt -o consumer
$ ./producer & ./consumer
```
https://github.com/dadoge217/OS/blob/210f58ca777751ac86af85860fb31793c75d13ee/producer-consumer/2025-03-28%2016-49-13.mkv
