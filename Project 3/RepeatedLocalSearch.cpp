// Zack Lee
// RepeatedLocalSearch.cpp
// repated local search to run the local search algorithm with the outerloop dependencies 

#include "RepeatedLocalSearch.h"
#include "Population.h"
#include <chrono>

// Function to perform repeated local search
algorithmOutput repeatedLocalSearch(int n, int m, float lower, float upper, int problemType, float alpha, int repeats, const std::string& csvFile, bool append) {
    
    // Output structure
    algorithmOutput out{};

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Store the best result across all repeats
    bool haveBest = false;
    Population::bestResult globalBest;
    
    // Repeat the local search process
    for (int r = 0; r < repeats; ++r) {

        // Initialize population
        Population pop(n, m);
        pop.generate(lower, upper);

        // Evaluate initial population
        for(int i = 0; i < pop.size(); ++i){

            // Improve each individual in the population once
            auto x_t = pop.getRow(i);
            auto x_new = improveOnce(pop, x_t, problemType, alpha, lower, upper);
            pop.setRow(i, x_new);
        }
    
    // Evaluate population after improvement
    pop.evaluate(problemType);
    auto bestRun = pop.best();

    // Update global best if necessary
    if (!haveBest || bestRun.fitness < globalBest.fitness) {
        globalBest = bestRun;
        haveBest = true;
    }
    }

    // Stop timer
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;

    // Write best result to CSV
    Population dummy(1, m); // dummy population to use writeBestCSV
    dummy.writeBestCSV(csvFile, append, globalBest, static_cast<float>(duration.count()));

    // Prepare output
    out.bestFitness = globalBest.fitness;
    out.runtimeMs = static_cast<float>(duration.count());
    return out;

}