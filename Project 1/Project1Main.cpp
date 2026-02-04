// Zack Lee
// Project1Main.cpp
// 01/14/2026

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <chrono>
#include <cctype>
#include <iomanip>

#include "Population.h"

struct Bounds {
    float min;
    float max;
};

// Skip blank lines or comment lines (# ...) in the txt files for the ranges
static bool skipLine(const std::string& line) {
    for (char c : line) {
        if (c == '#') return true;
        if (!std::isspace(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    try {
        
        // argument check for inputs
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " <problemType 1-10> <csvFile> <appendFlag 0/1>\n";
            return 1;
        }

        const int problemType = std::stoi(argv[1]);
        const std::string csvFile = argv[2];
        const int appendFlagInt = std::stoi(argv[3]);
        const bool append = (appendFlagInt != 0);

        if (problemType < 1 || problemType > 10) {
            throw std::runtime_error("Problem type must be between 1 and 10.");
        }
        if (appendFlagInt != 0 && appendFlagInt != 1) {
            throw std::runtime_error("appendFlag must be 0 (overwrite) or 1 (append).");
        }

        // reading the input file for m, n, and bounds
        std::ifstream in("InputFile.txt");
        if (!in) {
            throw std::runtime_error("Could not open InputFile.txt (must be in the same folder you run from).");
        }

        int m = -1;
        int n = 30; // default for project
        std::unordered_map<int, Bounds> bounds;

        // Read through the file line by line
        std::string line;
        while (std::getline(in, line)) {
            if (skipLine(line)) continue;

            std::istringstream iss(line);
            std::string first;
            iss >> first;

            if (first == "m" || first == "M") {
                iss >> m;
                continue;
            }
            if (first == "n" || first == "N") {
                iss >> n;
                continue;
            }

            int pid = std::stoi(first);
            float lo, hi;
            if (!(iss >> lo >> hi)) {
                throw std::runtime_error("Bad bounds line. Expected: <problemId> <low> <high>");
            }
            bounds[pid] = { lo, hi };
        }

        // edge case checks
        if (m != 10 && m != 20 && m != 30) {
            throw std::runtime_error("m must be 10, 20, or 30 in InputFile.txt (example: m 20).");
        }
        if (n <= 0) {
            throw std::runtime_error("n must be positive.");
        }
        if (bounds.find(problemType) == bounds.end()) {
            throw std::runtime_error("No bounds found in InputFile.txt for the selected problem type.");
        }

        const float lower = bounds[problemType].min;
        const float upper = bounds[problemType].max;

        // timer to measure runtime
        auto start = std::chrono::high_resolution_clock::now();

        Population pop(n, m);
        pop.generate(lower, upper);
        pop.evaluate(problemType);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> runtime = end - start;

        pop.writeFitnessCSV(csvFile, append, static_cast<float>(runtime.count()));

        // Get the runtimes of each function and print the n value, the problem type, the csv file name, the append flag, and the runtime in milliseconds
        std::cout << std::fixed << std::setprecision(6)
                  << " n=" << n
                  << " problem=" << problemType
                  << " csv=" << csvFile
                  << " append=" << (append ? 1 : 0)
                  << " runtime_ms=" << runtime.count()
                  << "\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
}