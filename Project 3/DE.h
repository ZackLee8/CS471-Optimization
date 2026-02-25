// Zack Lee
// DE.h
// 02/18/2026

#ifndef DE_H
#define DE_H

#include "Population.h"
#include "BlindSearch.h"

// DE function declarations
algorithmOutput differentialEvolution( int NP, int D, float lower, float upper, int problemType, int strategy, float F, float CR, float lambda, int generations, const std::string& csvFile, bool append);

// Returns mutant vector v for a given strategy.
// pop[k] is x_k, best is x_best, xi is x_i.
std::vector<float> makeMutant(
    int strategy,
    const std::vector<std::vector<float>>& pop,
    const std::vector<float>& xi,
    const std::vector<float>& best,
    int r1,int r2,int r3,int r4,int r5,
    float F, float lambda
);


#endif