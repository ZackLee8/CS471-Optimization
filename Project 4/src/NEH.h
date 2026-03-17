// Zack Lee
// 03/09/2026
// NEH.h
// Header file for the NEH class, which implements the NEH heuristic for job scheduling.

#ifndef NEH_H
#define NEH_H

#include "core.h"

#include <random>
#include <vector>


class NEH {
public:
    result solve(const instance& inst, problemType type) const;

private:
    std::vector<int> buildJobOrder(const instance& inst) const;
    std::vector<int> buildSequence(const instance& inst,
                                   const std::vector<int>& orderedJobs,
                                   problemType type,
                                   std::mt19937& randomEngine) const;
    int totalProcessingTime(const instance& inst, int job) const;
    std::vector<int> insertJobAtPosition(const std::vector<int>& sequence, int job, int position) const;
};

#endif // NEH_H
