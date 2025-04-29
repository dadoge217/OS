// Colin Grant
// banker.cpp
// 4/24/25

#include "banker.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Too few arguments, include the path to the resource table" << std::endl;
        exit(1);
    }
    char* resourceTable = argv[1];
    int allocated[NUMPROCESSES][NUMRESOURCES];
    int max[NUMPROCESSES][NUMRESOURCES];
    int available[NUMRESOURCES];
 
    processTables(resourceTable, allocated, max, available); // Reads in and creates tables from input file
 
    int finished[NUMPROCESSES], answer[NUMPROCESSES], index = 0;
 
    for (int i = 0; i < NUMPROCESSES; ++i) {
         finished[i] = 0;  // Initializes the array
    }

    int need[NUMPROCESSES][NUMRESOURCES];
 
    for (int i = 0; i < NUMPROCESSES; ++i) {
        for (int j = 0; j < NUMRESOURCES; ++j) {
            need[i][j] = max[i][j] - allocated[i][j]; // Finds the needed for each resource for each process
        }
    }

    for (int k = 0; k < NUMPROCESSES; ++k) {
        for (int currProcess = 0; currProcess < NUMPROCESSES; ++currProcess) { // Loops through the process in each process to find order
            if (finished[currProcess] == 0) { // Check if each process is finished
                int flag = 0;
                for (int currResource = 0; currResource < NUMRESOURCES; ++currResource) {
                    if (need[currProcess][currResource] > available[currResource]) { // If there's not enough resources, breaks and moves on
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {
                    answer[index++] = currProcess; // Adds the process to the solution list
                    for (int finishedProcess = 0; finishedProcess < NUMRESOURCES; ++finishedProcess) {
                        available[finishedProcess] += allocated[currProcess][finishedProcess]; // Deallocates resources from process
                        finished[currProcess] = 1; // Marks as finished
                    }
                }
            }
        }
    }

    int safeFlag = 1;

    for (int currProcess = 0; currProcess < NUMPROCESSES; ++currProcess) {
        if (finished[currProcess] == 0) { // Ensures all processes have finished using resources
            safeFlag = 0;
            std::cout << "The given sequence is not safe!!!" << std::endl;
            break;
        }
    }
    if (safeFlag == 1) { // Outputs sequence if in a safe state
        std::cout << "The following sequence is safe" << std::endl;
        for (int currProcess = 0; currProcess < NUMPROCESSES - 1; ++currProcess) {
            std::cout << " P" << answer[currProcess] << " ->";
        }
        std::cout << " P" << answer[NUMPROCESSES - 1] << std::endl;
    }
 
 
    return 0;
}

void processTables(char* file, int allocated[][NUMRESOURCES], int max[][NUMRESOURCES], int* available) {

    int currProcess =  0;
    int currResPosition = 0;
    std::string currentLine;
    std::ifstream resourceTable(file, std::ifstream::in);
    
    
    while(!resourceTable.eof()) {
        std::getline(resourceTable, currentLine, ENDLINE);
        if (std::stoi(currentLine) < 0) { //Available resources line
            for (int i = 0; i < NUMRESOURCES; ++i) {
                std::getline(resourceTable, currentLine, ENDLIST); // Gets each free resource count
                available[i] = std::stoi(currentLine);
            }
            break;
        }

        currProcess = std::stoi(currentLine);
    
        for (currResPosition = 0; currResPosition < NUMRESOURCES; ++currResPosition){ // Assigns allocated values first
            if(currResPosition == (NUMRESOURCES - 1)){
                std::getline(resourceTable, currentLine, ENDLINE); // Ends at ';'
            }
            else{
                std::getline(resourceTable, currentLine, ENDLIST); //Ends at ','
            }
            allocated[currProcess][currResPosition] = std::stoi(currentLine); // Assigns index of allocated to read value
        }
        for (currResPosition = 0; currResPosition < NUMRESOURCES; ++currResPosition) { // Max resources last in file so last assigned
            if(currResPosition == (NUMRESOURCES - 1)){ //At end
                std::getline(resourceTable, currentLine, ENDLINE); // Ends at ';'
            }
            else{
                std::getline(resourceTable, currentLine, ENDLIST); // Ends at ','
            }
            max[currProcess][currResPosition] = std::stoi(currentLine); // Assigns index of max to read value
        }
    }
}
