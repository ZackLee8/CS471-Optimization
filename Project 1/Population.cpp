// Zack Lee
// Population.cpp
// 01/14/2026

#include "Population.h"
#include "Functions.h"

// Constructor to initialize population size and dimension
Population::Population(int n, int m)
    : n_(n), m_(m),
      pop_(n, std::vector<float>(m)),
      fitness_(n, 0.0f) 
    {}

// populate the maxtrix/vector using the mersenne twister radnom generator
void Population::generate(float lower, float upper)
{
    // Seed changes every program run based on current time
    unsigned seed =
        static_cast<unsigned>(
            std::chrono::high_resolution_clock::now()
                .time_since_epoch()
                .count()
        );

    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(lower, upper);

    for (int i = 0; i < n_; ++i) {
        for (int j = 0; j < m_; ++j) {
            pop_[i][j] = dist(gen);
        }
    }
}


// switch case to select the correct function to evaluate
float Population::evalOne(const std::vector<float>& x, int problemType) const
{
    switch (problemType) {
        case 1:  return schwefelFunction(x);
        case 2:  return dejongFunction(x);
        case 3:  return rosenbrockFunction(x);
        case 4:  return rastriginFunction(x);
        case 5:  return griewankFunction(x);
        case 6:  return sineEnvelopeSineWaveFunction(x);
        case 7:  return stretchedVSineWaveFunction(x);
        case 8:  return ackleyOneFunction(x);
        case 9:  return ackleyTwoFunction(x);
        case 10: return eggHolderFunction(x);
        default:
            throw std::runtime_error("Invalid problem type (must be 1..10)");
    }
}

// evaluate all rows in the population matrix
void Population::evaluate(int problemType)
{
    // Evaluate each row and store fitness
    for (int i = 0; i < n_; ++i) {
        fitness_[i] = evalOne(pop_[i], problemType);
    }
}

// write the fitness vector to a csv file
void Population::writeFitnessCSV(const std::string& filename, bool append, float runtime_ms) const
{
    std::ofstream out;
    out.open(filename, append ? (std::ios::out | std::ios::app) : std::ios::out);

    if (!out) {
        throw std::runtime_error("Failed to open CSV file");
    }

     if (!append) {
        out << "fitness,runtime_ms\n";
    }

    // Write fitness values to file
    for (int i = 0; i < n_; ++i) {
        out << fitness_[i] << "," << runtime_ms <<"\n";
    }
}
