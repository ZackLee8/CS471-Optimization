// Zack Lee
// 03/09/2026
// core.h

#ifndef CORE_H
#define CORE_H
#include <string>
#include <vector>


// Problem types
enum class problemType {
    FSS, // Flow Shop Scheduling
    FSSb, // Flow Shop Scheduling with blocking
};

// Solution methods
enum class solutionMethod {
    NEH, // Nawaz-Enscore-Ham
    ACO, // Ant Colony Optimization
};

// Instance structure
struct instance {
    std::string name;
    int numJobs;
    int numMachines;
    std::vector<std::vector<int>> processingTimes; // processingTimes[job][machine]
};

// Result structure
struct result {
    std::string instanceName;
    problemType problem;
    solutionMethod method;
    std::vector<int> sequence;
    int makespan; // total completion time
    double executionTime; // in seconds
};

// Function declarations
instance loadInstance(const std::string& filename);
std::vector<std::string> getInstanceFiles(const std::string& directory);

// Makespan calculation functions
int makespanFSS(const instance& inst, const std::vector<int>& sequence);
int makespanFSSb(const instance& inst, const std::vector<int>& sequence);
int evaluateSequence(const instance& inst, const std::vector<int>& sequence, problemType type);

// Solution methods
std::string problemTypeToString(problemType type);
std::string solutionMethodToString(solutionMethod method);

#endif // CORE_H
