// Zack Lee
// Population.cpp
// 02/18/2026

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

// find the best vector and its fitness 
Population::bestResult Population::best() const{
    
    // check if there are currently vectors
    if (n_ <= 0){
        throw std::runtime_error("Population is empty");
    }

    // pick an initial best 
    int bestIndx = 0;
    float bestFit = fitness_[0];

    for (int i = 1; i < n_; ++i){
        if(fitness_[i] < bestFit){
            bestFit = fitness_[i];
            bestIndx = i;
        }
    }

        bestResult r;
        r.x = pop_[bestIndx];
        r.fitness = bestFit;
        r.index = bestIndx;
        return r;
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

// write the best fitness vector to a csv file
void Population::writeBestCSV(const std::string& filename, bool append, const bestResult& best, float runtime_ms) const {
    std::ofstream out;
    out.open(filename, append ? (std::ios::out | std::ios::app) : std::ios::out);

    if (!out) throw std::runtime_error("Failed to open CSV file");

    if (!append) {
        out << "best_fitness,runtime_ms,vector\n";
    }

    out << best.fitness << "," << runtime_ms << ",";

    for (size_t i = 0; i < best.x.size(); ++i) {
        out << best.x[i];
        if (i < best.x.size() - 1) out << " ";
    }
    out << "\n";
}

// evaluate a single vector using the selected problem
float Population::evaluateVector(const std::vector<float>& x, int problemType) const{
    return evalOne(x, problemType);
}

int Population::size() const { return n_; }

const std::vector<float>& Population::getRow(int i) const {
    if (i < 0 || i >= n_) throw std::out_of_range("row index");
    return pop_[i];
}

void Population::setRow(int i, const std::vector<float>& x) {
    if (i < 0 || i >= n_) throw std::out_of_range("row index");
    if ((int)x.size() != m_) throw std::runtime_error("dimension mismatch");
    pop_[i] = x;
}

