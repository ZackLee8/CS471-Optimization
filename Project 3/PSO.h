// Zack Lee
// PSO.h
// 02/18/2026

#ifndef PSO_H
#define PSO_H

#include <string>
#include "Population.h"
#include "BlindSearch.h"

// PSO function declaration
algorithmOutput particleSwarmOptimization(int particles, int m, float lower, float upper, int problemType, int generations, float c1, float c2, const std::string& csvFile, bool append);

#endif // PSO_H
