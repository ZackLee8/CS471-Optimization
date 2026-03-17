// Zack Lee
// 03/09/2026
// ACO.h
// Header file for the Ant Colony Optimization (ACO) algorithm implementation.

#ifndef ACO_H
#define ACO_H

#include "core.h"

#include <random>
#include <vector>

// Create the ACO class that encapsulates the Ant Colony Optimization algorithm for solving scheduling problems.
class ACO {
public:
    ACO(int numAnts = 20,
        int maxIterations = 50,
        double alpha = 1.0,
        double beta = 2.0,
        double evaporationRate = 0.1,
        double pheromoneDeposit = 100.0,
        double initialPheromone = 1.0,
        unsigned int randomSeed = 471);

    result solve(const instance& inst, problemType type) const;

// The ACO class includes parameters for controlling the behavior of the algorithm, 
// such as the number of ants, the maximum number of iterations, the influence of pheromone 
// and heuristic information, the evaporation rate, and the amount of pheromone deposited. 
// The solve method takes an instance of the scheduling problem and the problem type, and 
// returns a result containing the best solution found by the algorithm.
private:
    struct antSolution {
        std::vector<int> sequence;
        int makespan = 0;
    };

    // Parameters for the ACO algorithm
    int numAnts;
    int maxIterations;
    double alpha;
    double beta;
    double evaporationRate;
    double pheromoneDeposit;
    double initialPheromone;
    unsigned int randomSeed;

    // Initialize the pheromone levels for all edges in the solution construction graph.
    std::vector<std::vector<double>> initializePheromone(int numJobs) const;
    antSolution constructSolution(const instance& inst, problemType type, const std::vector<std::vector<double>>& pheromone, std::mt19937& randomEngine) const;

    // Select the next job for an ant to schedule based on the pheromone levels and heuristic information.
    int selectNextJob(const instance& inst, int previousNode, const std::vector<bool>& scheduled, const std::vector<std::vector<double>>& pheromone, std::mt19937& randomEngine) const;

    // Calculate the visibility (heuristic information) for a given job, which is typically
    // based on the total processing time of the job across all machines. A shorter processing 
    // time results in higher visibility, making it more attractive for selection by the ants.
    double visibility(const instance& inst, int job) const;

    // Calculate the total processing time for a given job across all machines
    int totalProcessingTime(const instance& inst, int job) const;

    // Evaporate pheromone on all edges to simulate the natural evaporation process
    void evaporatePheromone(std::vector<std::vector<double>>& pheromone) const;

    // Deposit pheromone based on the quality of the solutions found by the ants
    void depositPheromone(std::vector<std::vector<double>>& pheromone,
                          const std::vector<antSolution>& antSolutions) const;
};

#endif // ACO_H
