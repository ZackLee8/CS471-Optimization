// Zack Lee
// LocalSearch.h

#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include "Population.h"
#include "BlindSearch.h"

// Forward declaration
class Population;

// Perform one local step to generate new candidate solution
std::vector<float> localStep(const Population& pop, const std::vector<float>& x_i, int problemType, float alpha, float lowerBound, float upperBound);

// Improve once
std::vector<float> improveOnce(const Population& pop, const std::vector<float>& x_t, int problemType, float alpha, float lowerBound, float upperBound);

// Perform local search algorithm
algorithmOutput localSearch(int n, int m, float lower, float upper, int problemType, float alpha, const std::string& csvFile, bool append);
                            
#endif