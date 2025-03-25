# Producer/Consumer
# Description
This project is a solution to the producer/consumer problem. The producer generates a random number between 1-100 and places it in shared memory, which can only hold two items at a time When shared memory is full, producer calls the consumer procedure using a semaphore.
# Compilation
The consumer and producer are separate files. To compile, run:
`$ g++ producer.cpp -pthread -lrt -o producer\n`
`$ g++ consumer.cpp -pthread -lrt -o consumer\n`
`$ ./producer & ./consumer &`
