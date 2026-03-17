// Zack Lee
// 03/09/2026
// NEH.cpp
// Implementation of the NEH heuristic for job scheduling.

#include "NEH.h"

#include <algorithm>
#include <chrono>
#include <limits>
#include <numeric>
#include <random>

// Solves the job scheduling problem using the NEH heuristic. It constructs a sequence of jobs based 
// on their total processing times and iteratively builds the sequence by inserting jobs in the position 
// that minimizes the makespan. The function returns a result struct containing the instance name, problem 
// type, solution method, job sequence, makespan, and execution time.
result NEH::solve(const instance& inst, problemType type) const {
    const auto startTime = std::chrono::steady_clock::now();
    std::mt19937 randomEngine(471);

    // Initialize the result struct with instance details and solution method.
    result output{};
    output.instanceName = inst.name;
    output.problem = type;
    output.method = solutionMethod::NEH;

    // Handle edge cases where there are no jobs or machines, resulting in a makespan of 0 and execution time of 0.
    if (inst.numJobs <= 0 || inst.numMachines <= 0) {
        output.makespan = 0;
        output.executionTime = 0.0;
        return output;
    }

    // Build the job order based on total processing times and construct the job sequence using the NEH heuristic.
    const std::vector<int> orderedJobs = buildJobOrder(inst);
    output.sequence = buildSequence(inst, orderedJobs, type, randomEngine);
    output.makespan = evaluateSequence(inst, output.sequence, type);

    // Calculate the execution time by measuring the duration from the start to the end of the algorithm.
    const auto endTime = std::chrono::steady_clock::now();
    output.executionTime =
        std::chrono::duration<double>(endTime - startTime).count();

    return output;
}

// Build a list of job indices ordered by their total processing times in descending order.
std::vector<int> NEH::buildJobOrder(const instance& inst) const {
    std::vector<int> jobs(inst.numJobs);
    std::iota(jobs.begin(), jobs.end(), 0);

    // Sort the jobs based on their total processing times, with ties broken by job index to ensure a stable order.
    std::stable_sort(jobs.begin(), jobs.end(),
        [&inst, this](int leftJob, int rightJob) {
            const int leftTotal = totalProcessingTime(inst, leftJob);
            const int rightTotal = totalProcessingTime(inst, rightJob);

            // Sort in descending order of total processing time. If there is a tie, sort by job index in ascending order.
            if (leftTotal != rightTotal) {
                return leftTotal > rightTotal;
            }

            // If total processing times are equal, sort by job index in ascending order to maintain a stable order.
            return leftJob < rightJob;
        });

    return jobs;
}

// Build the job sequence using the NEH heuristic by iteratively inserting jobs in the position that minimizes the makespan.
std::vector<int> NEH::buildSequence(const instance& inst,
                                    const std::vector<int>& orderedJobs,
                                    problemType type,
                                    std::mt19937& randomEngine) const {
    
    // Handle edge cases where there are no jobs or only one job, returning the appropriate sequence.
    if (orderedJobs.empty()) {
        return {};
    }

    // If there is only one job, return that job as the sequence.
    if (orderedJobs.size() == 1) {
        return {orderedJobs[0]};
    }

    // Start with the first two jobs and determine the best sequence for these two jobs by evaluating both possible orders.
    std::vector<int> sequenceOne = {orderedJobs[0], orderedJobs[1]};
    std::vector<int> sequenceTwo = {orderedJobs[1], orderedJobs[0]};

    // Evaluate both sequences and choose randomly when the makespans tie.
    const int makespanOne = evaluateSequence(inst, sequenceOne, type);
    const int makespanTwo = evaluateSequence(inst, sequenceTwo, type);
    std::vector<int> bestSequence = sequenceOne;
    if (makespanTwo < makespanOne) {
        bestSequence = sequenceTwo;
    } else if (makespanTwo == makespanOne) {
        std::uniform_int_distribution<int> tieBreaker(0, 1);
        bestSequence = (tieBreaker(randomEngine) == 0) ? sequenceOne : sequenceTwo;
    }

    // Iteratively insert the remaining jobs into the best sequence at the position that minimizes the makespan.
    for (std::size_t index = 2; index < orderedJobs.size(); ++index) {
        const int jobToInsert = orderedJobs[index];
        int bestMakespan = std::numeric_limits<int>::max();
        std::vector<std::vector<int>> bestInsertions;

        // Try inserting the current job at every possible position in the best sequence and evaluate the makespan for each candidate sequence.
        for (std::size_t position = 0; position <= bestSequence.size(); ++position) {
            std::vector<int> candidate =
                insertJobAtPosition(bestSequence, jobToInsert, static_cast<int>(position));
            const int candidateMakespan = evaluateSequence(inst, candidate, type);

            // Keep every best insertion so ties can be broken randomly.
            if (candidateMakespan < bestMakespan) {
                bestMakespan = candidateMakespan;
                bestInsertions.clear();
                bestInsertions.push_back(candidate);
            } else if (candidateMakespan == bestMakespan) {
                bestInsertions.push_back(candidate);
            }
        }

        std::uniform_int_distribution<std::size_t> tieBreaker(0, bestInsertions.size() - 1);
        bestSequence = bestInsertions[tieBreaker(randomEngine)];
    }

    // Return the final best sequence after all jobs have been inserted.
    return bestSequence;
}

// Calculate the total processing time for a given job by summing the processing times across all machines.
int NEH::totalProcessingTime(const instance& inst, int job) const {
    int totalTime = 0;

    // Sum the processing times for the specified job across all machines to get the total processing time.
    for (int machine = 0; machine < inst.numMachines; ++machine) {
        totalTime += inst.processingTimes[job][machine];
    }

    // Return the total processing time for the specified job.
    return totalTime;
}

// Insert a job into a given sequence at the specified position and return the new sequence.
std::vector<int> NEH::insertJobAtPosition(const std::vector<int>& sequence, int job, int position) const {
    std::vector<int> candidate = sequence;
    candidate.insert(candidate.begin() + position, job);
    return candidate;
}
