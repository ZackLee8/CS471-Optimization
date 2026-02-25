// Zack Lee
//BlindSearch.h

#ifndef BLINDSEARCH_H
#define BLINDSEARCH_H

#include <string>

// Struct to hold algorithm output
struct algorithmOutput {
    float bestFitness; 
    float runtimeMs;
};

// Function declaration for blind search
algorithmOutput blindSearch(int n, int m, float lower, float upper, int problemType, const std::string& csvFile, bool append);

#endif