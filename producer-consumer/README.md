# Producer/Consumer
# Description
This project is a solution to the producer/consumer problem. The producer generates random numbers between 1-100 and places them in shared memory, which can only hold two items at a time. When shared memory is full, producer calls the consumer procedure using a semaphore. After the consumer consumes these values, it flips the semaphore and returns control of the critical section to the producer. This cycle repeats until the end of the loop.
# Compilation
The consumer and producer are separate files. To compile, run:
`$ g++ producer.cpp -pthread -lrt -o producer\n`
`$ g++ consumer.cpp -pthread -lrt -o consumer\n`
`$ ./producer & ./consumer`
