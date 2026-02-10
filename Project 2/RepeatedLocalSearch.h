// Zack Lee
// ReapeatedLocalSearch.h

#ifndef REPEATEDLOCALSEARCH_H
#define REPEATEDLOCALSEARCH_H
#include "Population.h"
#include "LocalSearch.h"
#include "BlindSearch.h"

// Function declaration for repeated local search
algorithmOutput repeatedLocalSearch(int n, int m, float lower, float upper, int problemType, float alpha, int repeats, const std::string& csvFile, bool append);

#endif