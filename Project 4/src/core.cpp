// Zack Lee
// 03/09/2026
// core.cpp

#include "core.h"

// C++17 filesystem library is used for directory traversal and file handling
#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

// For convenience, we alias the filesystem namespace
namespace fs = std::experimental::filesystem;

// Function definitions
instance loadInstance(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open instance file: " + filename);
    }

    // Read instance header and processing times
    instance inst;
    inst.name = fs::path(filename).filename().string();

    // The first line of the instance file should contain the number of machines and jobs
    inputFile >> inst.numMachines >> inst.numJobs;
    if (!inputFile) {
        throw std::runtime_error("Invalid instance header in file: " + filename);
    }

    // Initialize the processing times matrix
    inst.processingTimes.assign(inst.numJobs, std::vector<int>(inst.numMachines, 0));

    // Read processing times for each job and machine
    for (int machine = 0; machine < inst.numMachines; ++machine) {
        for (int job = 0; job < inst.numJobs; ++job) {
            inputFile >> inst.processingTimes[job][machine];
            if (!inputFile) {
                throw std::runtime_error("Invalid processing-time data in file: " + filename);
            }
        }
    }

    return inst;
}

// Retrieves all instance files from the specified directory, filtering for .txt files and sorting them by their numeric prefix
std::vector<std::string> getInstanceFiles(const std::string& directory) {
    std::vector<std::string> files;

    // Iterate through the directory and collect .txt files
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }

    // Sort files based on their numeric prefix (e.g., "1.txt", "2.txt", etc.)
    std::sort(files.begin(), files.end(),
        [](const std::string& left, const std::string& right) {
            return std::stoi(fs::path(left).stem().string()) <
                   std::stoi(fs::path(right).stem().string());
        });

    return files;
}

// Makespan calculation for Flow Shop Scheduling (FSS)
int makespanFSS(const instance& inst, const std::vector<int>& sequence) {
    if (sequence.empty()) {
        return 0;
    }

    // If there's only one machine, the makespan is simply the sum of processing times for the scheduled jobs
    const int scheduledJobs = static_cast<int>(sequence.size());
    std::vector<std::vector<int>> completionTimes(
        inst.numMachines, std::vector<int>(scheduledJobs, 0));

    // Initialize completion times for the first job in the sequence
    const int firstJob = sequence[0];
    completionTimes[0][0] = inst.processingTimes[firstJob][0];

    // Calculate completion times for the first job across all machines
    for (int machine = 1; machine < inst.numMachines; ++machine) {
        completionTimes[machine][0] =
            completionTimes[machine - 1][0] + inst.processingTimes[firstJob][machine];
    }

    // Calculate completion times for the remaining jobs in the sequence
    for (int position = 1; position < scheduledJobs; ++position) {
        const int job = sequence[position];
        completionTimes[0][position] =
            completionTimes[0][position - 1] + inst.processingTimes[job][0];
    }

    // Calculate completion times for the remaining machines and jobs
    for (int machine = 1; machine < inst.numMachines; ++machine) {
        for (int position = 1; position < scheduledJobs; ++position) {
            const int job = sequence[position];
            completionTimes[machine][position] =
                std::max(completionTimes[machine - 1][position],
                         completionTimes[machine][position - 1]) +
                inst.processingTimes[job][machine];
        }
    }

    // The makespan is the completion time of the last job on the last machine
    return completionTimes[inst.numMachines - 1][scheduledJobs - 1];
}

// Makespan calculation for Flow Shop Scheduling with blocking (FSSb)
int makespanFSSb(const instance& inst, const std::vector<int>& sequence) {
    if (sequence.empty()) {
        return 0;
    }

    // If there's only one machine, the makespan is simply the sum of processing times for the scheduled jobs
    if (inst.numMachines == 1) {
        int totalTime = 0;
        for (int job : sequence) {
            totalTime += inst.processingTimes[job][0];
        }
        return totalTime;
    }

    // For multiple machines, we need to account for blocking. We will track departure times from each machine for each job in the sequence.
    const int scheduledJobs = static_cast<int>(sequence.size());
    std::vector<std::vector<int>> departureTimes(
        inst.numMachines, std::vector<int>(scheduledJobs, 0));

    // Initialize departure times for the first job in the sequence
    const int firstJob = sequence[0];
    departureTimes[0][0] = inst.processingTimes[firstJob][0];

    // Calculate departure times for the first job across all machines
    for (int machine = 1; machine < inst.numMachines; ++machine) {
        departureTimes[machine][0] =
            departureTimes[machine - 1][0] + inst.processingTimes[firstJob][machine];
    }

    // Calculate departure times for the remaining jobs in the sequence
    for (int position = 1; position < scheduledJobs; ++position) {
        const int job = sequence[position];

        // For the first machine, the departure time is the maximum of the previous job's departure time on the same machine and the previous job's departure time on the next machine (due to blocking)
        departureTimes[0][position] = std::max(departureTimes[0][position - 1] + inst.processingTimes[job][0], departureTimes[1][position - 1]);

        // For the intermediate machines, the departure time is the maximum of the previous job's departure time on the same machine and the previous job's departure time on the next machine (due to blocking)
        for (int machine = 1; machine < inst.numMachines - 1; ++machine) {
            departureTimes[machine][position] = std::max(departureTimes[machine - 1][position] + inst.processingTimes[job][machine], departureTimes[machine + 1][position - 1]);
        }

        // For the last machine, the departure time is simply the previous job's departure time on the same machine plus the processing time (no blocking after the last machine)
        departureTimes[inst.numMachines - 1][position] = departureTimes[inst.numMachines - 2][position] + inst.processingTimes[job][inst.numMachines - 1];
    }

    // The makespan is the departure time of the last job on the last machine
    return departureTimes[inst.numMachines - 1][scheduledJobs - 1];
}

// Selects the correct makespan evaluator based on the chosen scheduling model
int evaluateSequence(const instance& inst, const std::vector<int>& sequence, problemType type) {
    if (type == problemType::FSS) {
        return makespanFSS(inst, sequence);
    }

    return makespanFSSb(inst, sequence);
}

// Utility functions to convert enums to strings for better readability in results
std::string problemTypeToString(problemType type) {
    return (type == problemType::FSS) ? "FSS" : "FSSB";
}

// Converts the solution method enum to a string for better readability in results
std::string solutionMethodToString(solutionMethod method) {
    return (method == solutionMethod::NEH) ? "NEH" : "ACO";
}
