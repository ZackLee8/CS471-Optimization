// Zack Lee
// Population.h
// 01/14/2026

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <stdexcept>
#include <chrono>

class Population {
public:

    // n = 30, m = 10, 20, or 30
    Population(int n, int m);

    // Generate population values uniformly in [min, max]
    void generate(float lower, float upper);

    // Evaluate ONLY the chosen problem (1..10) for all 30 rows
    void evaluate(int problemType);

    // Write fitness values (one per row) to CSV
    void writeFitnessCSV(const std::string &filename, bool append, float runtime_ms) const;

   

private:
    int n_;
    int m_;

    // Population matrix (n x m)
    std::vector<std::vector<float>> pop_; 
    std::vector<float> fitness_;

    // Evaluate a single vector using the selected problem
    float evalOne(const std::vector<float>& x, int problemType) const;
};

#endif
