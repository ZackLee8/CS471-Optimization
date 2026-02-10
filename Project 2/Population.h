// Zack Lee
// Population.h
// 02/03/2026

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

    // struct to help store vectors and the fitness to find the best one 
    struct bestResult {
        std::vector<float> x;
        float fitness;
        int index;
    };

    bestResult best() const;

    // Write only the BEST fitness to the CSV out of the matrix
    void writeBestCSV(const std::string& filename, bool append, const bestResult& best, float runtime_ms) const;

    // Evaluate a single vector using the selected problem
    float evaluateVector(const std::vector<float>& x, int problemType) const;

    // Accessors
    const std::vector<float>& getRow(int i) const;
    void setRow(int i, const std::vector<float>& x);
    int size() const;



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
