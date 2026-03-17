// Zack Lee
// 03/09/2026
// ACO.cpp

#include "ACO.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <random>

// Implementation of the Ant Colony Optimization (ACO) algorithm for solving scheduling problems.
ACO::ACO(int numAnts,
         int maxIterations,
         double alpha,
         double beta,
         double evaporationRate,
         double pheromoneDeposit,
         double initialPheromone,
         unsigned int randomSeed)
    : numAnts(numAnts),
      maxIterations(maxIterations),
      alpha(alpha),
      beta(beta),
      evaporationRate(evaporationRate),
      pheromoneDeposit(pheromoneDeposit),
      initialPheromone(initialPheromone),
      randomSeed(randomSeed) {}

      // The solve method takes an instance of the scheduling problem and the problem type, and
result ACO::solve(const instance& inst, problemType type) const {
    const auto startTime = std::chrono::steady_clock::now();
    std::mt19937 randomEngine(randomSeed);

    // Initialize the result structure to store the best solution found by the algorithm, along with metadata about the instance and execution time
    result output{};
    output.instanceName = inst.name;
    output.problem = type;
    output.method = solutionMethod::ACO;

    // Handle edge cases where the instance has no jobs or machines, returning an empty solution with a makespan of 0 and an execution time of 0.0 seconds to indicate that no scheduling is needed
    if (inst.numJobs <= 0 || inst.numMachines <= 0) {
        output.makespan = 0;
        output.executionTime = 0.0;
        return output;
    }

    // Initialize the pheromone levels for all edges in the solution construction graph, and set up variables to track the best solution found by the ants during the iterations of the algorithm
    std::vector<std::vector<double>> pheromone = initializePheromone(inst.numJobs);
    antSolution globalBest;
    globalBest.makespan = std::numeric_limits<int>::max();

    // Main loop of the ACO algorithm, where each iteration simulates the behavior of a colony of ants constructing solutions based on pheromone levels and heuristic information, and then updates 
    // the pheromone levels based on the quality of the solutions found
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        std::vector<antSolution> antSolutions;
        antSolutions.reserve(numAnts);

        // Each ant constructs a solution by iteratively selecting the next job based on pheromone and heuristic information, and then evaluates the makespan of the constructed 
        // solution using the provided evaluation function. The best solution found by any ant in this iteration is compared to the global best solution, and if it is better, 
        // it becomes the new global best solution.
        for (int ant = 0; ant < numAnts; ++ant) {
            antSolution currentSolution =
                constructSolution(inst, type, pheromone, randomEngine);
            antSolutions.push_back(currentSolution);

            // Update the global best solution if the current solution found by this ant is better than the best solution found so far by any ant in previous iterations. 
            //This ensures that the algorithm retains the best solution found across all iterations
            if (currentSolution.makespan < globalBest.makespan) {
                globalBest = currentSolution;
            }
        }

        // After all ants have constructed their solutions for this iteration, the pheromone levels are updated to reflect the quality of the solutions found.
        // Pheromone evaporation is applied to all edges to simulate the natural evaporation process, which helps to prevent the algorithm from converging too 
        // quickly to a suboptimal solution by reducing the influence of older solutions. Then, pheromone is deposited on the edges based on the quality of the 
        // solutions found by the ants, with better solutions resulting in more pheromone being deposited, which increases the likelihood that those solutions will be selected by ants in future iterations.
        evaporatePheromone(pheromone);
        depositPheromone(pheromone, antSolutions);
    }

    // After the main loop of the ACO algorithm has completed, the best solution found by any ant across all iterations is stored in the output structure, 
    // along with the total execution time of the algorithm.
    output.sequence = globalBest.sequence;
    output.makespan = globalBest.makespan;

    // Record the end time and calculate the total execution time of the algorithm by taking the difference between the start and end times, and converting it to seconds.
    const auto endTime = std::chrono::steady_clock::now();
    output.executionTime =
        std::chrono::duration<double>(endTime - startTime).count();

    return output;
}

// Initialize the pheromone levels for all edges in the solution construction graph.
std::vector<std::vector<double>> ACO::initializePheromone(int numJobs) const {
    return std::vector<std::vector<double>>(
        numJobs + 1, std::vector<double>(numJobs, initialPheromone));
}

// Construct a solution for an ant by iteratively selecting the next job based on pheromone and heuristic information.
ACO::antSolution ACO::constructSolution(const instance& inst, problemType type, const std::vector<std::vector<double>>& pheromone, std::mt19937& randomEngine) const {
    antSolution solution;
    solution.sequence.reserve(inst.numJobs);

    // Keep track of which jobs have been scheduled to ensure that each job is scheduled exactly once
    std::vector<bool> scheduled(inst.numJobs, false);
    int previousNode = inst.numJobs; // Start node for the first job choice

    // Construct a solution by iteratively selecting the next job based on pheromone and heuristic information
    for (int position = 0; position < inst.numJobs; ++position) {
        const int nextJob =
            selectNextJob(inst, previousNode, scheduled, pheromone, randomEngine);

        // If selectNextJob returns an invalid job index (which should not happen in a valid instance), break the loop to prevent out-of-bounds access
        solution.sequence.push_back(nextJob);
        scheduled[nextJob] = true;
        previousNode = nextJob;
    }

    // Evaluate the makespan of the constructed solution using the provided evaluation function
    solution.makespan = evaluateSequence(inst, solution.sequence, type);
    return solution;
}

// Select the next job for an ant to schedule based on the pheromone levels and heuristic information.
int ACO::selectNextJob(const instance& inst,
                       int previousNode,
                       const std::vector<bool>& scheduled,
                       const std::vector<std::vector<double>>& pheromone,
                       std::mt19937& randomEngine) const {
    std::vector<int> candidateJobs;
    std::vector<double> probabilities;

    // Calculate the selection probabilities for each unscheduled job based on pheromone and visibility
    for (int job = 0; job < inst.numJobs; ++job) {
        if (scheduled[job]) {
            continue;
        }

        // Add the job to the list of candidates and calculate its selection probability
        candidateJobs.push_back(job);

        // To prevent numerical issues, ensure that pheromone and visibility values are not too small
        const double pheromoneValue = std::max(pheromone[previousNode][job], 1e-6);
        const double visibilityValue = std::max(visibility(inst, job), 1e-6);
        const double weight = std::pow(pheromoneValue, alpha) * std::pow(visibilityValue, beta);

        // If the weight is zero (which can happen if both pheromone and visibility are very small), assign a small positive value to ensure it can be selected
        probabilities.push_back(weight);
    }

    // If there are no candidate jobs (which should not happen in a valid instance), return a default value
    if (candidateJobs.empty()) {
        return 0;
    }

    // Normalize the probabilities to create a valid probability distribution for selection
    const double totalWeight = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);

    // If the total weight is zero (which can happen if all weights are very small), select a job uniformly at random from the candidates to ensure that the algorithm can continue making progress
    if (totalWeight <= 0.0) {
        std::uniform_int_distribution<int> randomChoice(
            0, static_cast<int>(candidateJobs.size()) - 1);
        return candidateJobs[randomChoice(randomEngine)];
    }

    // Normalize the probabilities to create a valid probability distribution for selection
    std::discrete_distribution<int> nextJobDistribution(
        probabilities.begin(), probabilities.end());
    return candidateJobs[nextJobDistribution(randomEngine)];
}

// Calculate the visibility (heuristic information) for a given job, which is typically
// based on the total processing time of the job across all machines. A shorter processing 
// time results in higher visibility, making it more attractive for selection by the ants.
double ACO::visibility(const instance& inst, int job) const {
    const int totalTime = totalProcessingTime(inst, job);
    return 1.0 / static_cast<double>(std::max(totalTime, 1));
}

// Calculate the total processing time for a given job across all machines
int ACO::totalProcessingTime(const instance& inst, int job) const {
    int totalTime = 0;

    for (int machine = 0; machine < inst.numMachines; ++machine) {
        totalTime += inst.processingTimes[job][machine];
    }

    return totalTime;
}

// Evaporate pheromone on all edges to simulate the natural evaporation process
void ACO::evaporatePheromone(std::vector<std::vector<double>>& pheromone) const {
    for (std::vector<double>& row : pheromone) {
        for (double& value : row) {
            value = std::max(1e-6, value * (1.0 - evaporationRate));
        }
    }
}

// Deposit pheromone based on the quality of the solutions found by the ants
void ACO::depositPheromone(std::vector<std::vector<double>>& pheromone,
                           const std::vector<antSolution>& antSolutions) const {
    const int startNode = static_cast<int>(pheromone.size()) - 1;

    for (const antSolution& solution : antSolutions) {
        if (solution.makespan <= 0) {
            continue;
        }

        const double depositAmount = pheromoneDeposit / solution.makespan;
        int previousNode = startNode;

        for (int job : solution.sequence) {
            pheromone[previousNode][job] += depositAmount;
            previousNode = job;
        }
    }
}
