// Zack Lee
// BlindSearch.cpp
// Blind search class to run the bind search

#include "BlindSearch.h"
#include "Population.h"
#include <chrono>

// Function to perform blind search
algorithmOutput blindSearch(int n, int m, float lower, float upper, int problemType, const std::string& csvFile, bool append){

    // Struct to hold output results
    algorithmOutput out{};

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Create and evaluate population
    Population pop(n, m);
    pop.generate(lower, upper);
    pop.evaluate(problemType);

    // Get the best result
    auto best = pop.best();

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> runtime = end - start;

    // Write best result to CSV
    pop.writeBestCSV(csvFile, append, best, static_cast<float>(runtime.count()));

    // Populate output struct
    out.bestFitness = best.fitness;
    out.runtimeMs = runtime.count();
    return out; 

}

