// Zack Lee
// 03/09/2026
// RunExperiments.cpp

#include "RunExperiments.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::experimental::filesystem;

// The RunExperiments class is responsible for orchestrating the execution of the NEH and ACO 
// algorithms across all instances in the specified data directory. It handles loading instances, 
// running the algorithms, collecting results, and writing output CSV files for analysis. The class 
// also includes utility functions for resolving directories, printing summaries, and formatting sequences for output.
RunExperiments::RunExperiments(const std::string& dataDirectory)
    : dataDirectory(dataDirectory),
      acoSolver(25, 60, 1.0, 3.0, 0.2, 150.0, 1.0, 471) {}

// The runAll method is the main entry point for executing the experiments. It resolves the data and output 
//directories, retrieves instance files, runs each instance through both algorithms and problem types, collects results, and writes them to CSV files.
std::vector<result> RunExperiments::runAll() const {
    const std::string resolvedDirectory = resolveDataDirectory();
    const std::string outputDirectory = resolveOutputDirectory(resolvedDirectory);
    const std::vector<std::string> instanceFiles = getInstanceFiles(resolvedDirectory);

    // Reserve space for results to avoid unnecessary reallocations during collection
    std::vector<result> allResults;
    allResults.reserve(instanceFiles.size() * 4);

    std::cout << "Running " << instanceFiles.size()
              << " instances across NEH and ACO for FSS and FSSB.\n\n";

    // Iterate through each instance file, run the experiments, and collect results
    for (std::size_t index = 0; index < instanceFiles.size(); ++index) {
        const std::string& instancePath = instanceFiles[index];
        const std::string instanceName = fs::path(instancePath).filename().string();

        // Print progress to the console for better visibility of the experiment execution
        std::cout << "[" << (index + 1) << "/" << instanceFiles.size() << "] "
                  << instanceName << '\n';

        // Run the instance through both algorithms and problem types, and collect results
        const std::vector<result> instanceResults = runInstance(instancePath);
        for (const result& currentResult : instanceResults) {
            printResultSummary(currentResult);
            allResults.push_back(currentResult);
        }

        std::cout << '\n';
    }

    // After all instances have been processed, write the collected results to CSV files for analysis
    writeCombinationCSVs(allResults, outputDirectory);

    // Print a final summary of the total runs and the location of the output files
    std::cout << "Finished " << allResults.size() << " total runs.\n";
    std::cout << "Saved CSV files to: " << outputDirectory << '\n';
    return allResults;
}

// Resolves the data directory by checking both the provided path and a fallback path. If neither exists, it throws an exception.
std::string RunExperiments::resolveDataDirectory() const {
    if (fs::exists(dataDirectory)) {
        return dataDirectory;
    }

    const std::string fallbackDirectory = "../" + dataDirectory;
    if (fs::exists(fallbackDirectory)) {
        return fallbackDirectory;
    }

    throw std::runtime_error("Could not find data directory: " + dataDirectory);
}

// Resolves the output directory by creating an "output" folder in the same parent directory as the data directory. If the output directory cannot be created, it throws an exception.
std::string RunExperiments::resolveOutputDirectory(const std::string& resolvedDataDirectory) const {
    fs::path dataPath(resolvedDataDirectory);

    if (!dataPath.is_absolute()) {
        dataPath = fs::current_path() / dataPath;
    }

    const fs::path outputPath = dataPath.parent_path() / "output";
    fs::create_directories(outputPath);
    return outputPath.string();
}

// Runs a single instance through both the NEH and ACO algorithms for both FSS and FSSB problem types, and collects the results in a vector.        
std::vector<result> RunExperiments::runInstance(const std::string& instancePath) const {
    const instance inst = loadInstance(instancePath);

    // Reserve space for results to avoid unnecessary reallocations during collection
    std::vector<result> resultsForInstance;
    resultsForInstance.reserve(4);

    // Run the instance through both algorithms and problem types, and collect results
    resultsForInstance.push_back(nehSolver.solve(inst, problemType::FSS));
    resultsForInstance.push_back(nehSolver.solve(inst, problemType::FSSb));
    resultsForInstance.push_back(acoSolver.solve(inst, problemType::FSS));
    resultsForInstance.push_back(acoSolver.solve(inst, problemType::FSSb));

    return resultsForInstance;
}

// Prints a summary of the result to the console, including the solution method, problem type, makespan, and execution time for better visibility during experiment execution.
void RunExperiments::printResultSummary(const result& output) const {
    std::cout << "  "
              << solutionMethodToString(output.method) << " | "
              << problemTypeToString(output.problem) << " | "
              << "Makespan: " << output.makespan << " | "
              << "Time: " << output.executionTime << " s\n";
}

// Writes the collected results to CSV files, creating separate files for each combination of solution method and problem type. The CSV files include headers and formatted sequences for better analysis.
void RunExperiments::writeCombinationCSVs(const std::vector<result>& allResults,
                                          const std::string& outputDirectory) const {
    for (solutionMethod method : {solutionMethod::NEH, solutionMethod::ACO}) {
        for (problemType problem : {problemType::FSS, problemType::FSSb}) {
            std::vector<result> filteredResults;

            // Filter results for the current combination of solution method and problem type
            for (const result& currentResult : allResults) {
                if (currentResult.method == method && currentResult.problem == problem) {
                    filteredResults.push_back(currentResult);
                }
            }

            if (filteredResults.empty()) {
                continue;
            }

            // Build the output file path using the method and problem type, and write the filtered results to a CSV file with appropriate headers and formatting
            const std::string tag = buildResultTag(filteredResults.front());
            const fs::path outputPath = fs::path(outputDirectory) / (tag + ".csv");
            std::ofstream outputFile(outputPath.string());
            if (!outputFile.is_open()) {
                throw std::runtime_error("Could not create file: " + outputPath.string());
            }

            // Write CSV headers and rows for the filtered results, including instance name, problem type, solution method, makespan, execution time, and 
            // formatted sequence for better readability in analysis
            outputFile << "instance,problem,method,makespan,execution_time_seconds,sequence\n";
            for (const result& currentResult : filteredResults) {
                outputFile << currentResult.instanceName << ','
                           << problemTypeToString(currentResult.problem) << ','
                           << solutionMethodToString(currentResult.method) << ','
                           << currentResult.makespan << ','
                           << currentResult.executionTime << ",\""
                           << sequenceToString(currentResult.sequence) << "\"\n";
            }
        }
    }
}

// Converts a sequence of job indices to a space-separated string format for better readability in CSV output and console summaries.
std::string RunExperiments::sequenceToString(const std::vector<int>& sequence) const {
    std::ostringstream builder;

    // Convert job indices to 1-based and format them as a space-separated string
    for (std::size_t index = 0; index < sequence.size(); ++index) {
        builder << (sequence[index] + 1);
        if (index + 1 < sequence.size()) {
            builder << ' ';
        }
    }

    return builder.str();
}

// Builds a result tag based on the solution method and problem type, converting them to lowercase for consistent file naming in the output directory.
std::string RunExperiments::buildResultTag(const result& output) const {
    std::string methodTag = solutionMethodToString(output.method);
    std::string problemTag = problemTypeToString(output.problem);

    // Convert tags to lowercase for consistent file naming
    std::transform(methodTag.begin(), methodTag.end(), methodTag.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });
    // Convert tags to lowercase for consistent file naming
    std::transform(problemTag.begin(), problemTag.end(), problemTag.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::tolower(character));
                   });

    return methodTag + "_" + problemTag;
}
